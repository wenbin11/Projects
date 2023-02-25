from django import forms
from django.forms import ModelForm
from .models import *
from django.contrib.auth.models import User

class UserForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput())

    class Meta:
        model = User
        fields = ['username', 'email', 'password']
    
    def clean_username(self):
        username = self.cleaned_data['username']
        if not username:
            raise forms.ValidationError('Username is required')
        if User.objects.filter(username=username).exists():
            raise forms.ValidationError('Username already in use')
        return username
    
    def clean_email(self):
        email = self.cleaned_data['email']
        if not email:
            raise forms.ValidationError('Email is required')
        
        name, domain = email.split('@')
        
        if User.objects.filter(email__startswith=name).exists():
            raise forms.ValidationError('Email already in use')
        
        return email
    
class UserProfileForm(forms.ModelForm):
    profile_pic = forms.ImageField(required=True)
    class Meta:
        model = AppUser
        fields = ['profile_pic']

class UserBioForm(forms.ModelForm):
    bio = forms.CharField(widget=forms.Textarea(attrs={'class': 'form-control'}))
    class Meta:
        model = AppUser
        fields = ['bio']
    
    def clean_bio(self):
        bio = self.cleaned_data['bio']
        if not bio:
            raise forms.ValidationError('bio is required')
        
        return bio
    

class PostForm(forms.ModelForm):
    post_content = forms.CharField(widget=forms.Textarea(attrs={'class': 'form-control'}))
    class Meta:
        model = Post
        fields = ['post_content']
    
    # add some form styling
    widgets = {
        'post_content': forms.Textarea(attrs={'class': 'form-control'}),
    }

    # check for empty post and raise exception
    def clean_post_content(self):
        post_content = self.cleaned_data.get('post_content')
        if not post_content:
            raise forms.ValidationError("Post content can't be empty.")
        return post_content

class ImageForm(forms.ModelForm):
    class Meta:
        model = Image
        fields = ['image']

    # add some form styling 
    widgets = {
        'image': forms.ClearableFileInput(attrs={'class': 'form-control-file'}),
    }

    # check for no empty file submission
    def clean(self):
        cleaned_data = super().clean()
        image = cleaned_data.get('image')

        if not image:
            raise forms.ValidationError('Please upload an image.')


class ChatMessageForm(forms.Form):
    message = forms.CharField(widget=forms.Textarea(attrs={'rows': 3}))

    def clean(self):
        cleaned_data = super().clean()
        message = cleaned_data.get('message')
        if not message:
            raise forms.ValidationError('Please enter a message')