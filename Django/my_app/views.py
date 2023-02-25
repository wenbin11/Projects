from django.shortcuts import render, redirect, get_object_or_404
from django.http import HttpResponseRedirect, Http404

from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required

from django.contrib import messages
from django.utils import timezone
from django.db.models import Q, Max

import os
from django.conf import settings

from .models import *
from django.contrib.auth.models import User
from .forms import *
from .serializers import *

# this is a function for the logged in user's home page
def index(request):
    if request.user.is_authenticated:
        # get the current user object for the logged in personnel
        app_user = request.user.appuser
        # get the user's information such as their post and images
        posts = Post.objects.filter(user=app_user)
        images = Image.objects.filter(user=app_user)

        return render(request, 'index.html', {'user': app_user,
                                              'posts': posts,
                                              'images': images
                                              })
    else:
        return render(request, 'index.html')

# get another user's page which a user has searched for 
def user_page(request, username):
    # attempt to find the username in the database
    try:
        user = AppUser.objects.get(user__username=username)
    # raise exception if not found in the database
    except User.DoesNotExist:
        raise Http404("User does not exist")

    # Get the posts and images associated with the user
    app_user = request.user

    posts = Post.objects.filter(user=user)
    images = Image.objects.filter(user=user)
    
    if FriendList.objects.filter(user1=user.user, user2=app_user).exists() or FriendList.objects.filter(user1=app_user, user2=user.user).exists():
        friend = True
    else:
        friend = False

    # Render the user's profile page with their posts and images
    return render(request, 'search.html', {'user': user, 
                                          'posts': posts, 
                                          'images': images,
                                          'friend': friend
                                          })

def search_user(request):
    # check if the request is 'GET'
    if request.method == 'GET':
        # get the username from the query parameters
        username = request.GET.get('username')
        # redirect to the user page view with the username parameter
        return redirect('user_page', username=username)

@login_required
def view_friends(request):
    # Get the current user
    user = request.user

    # Retrieve the FriendList objects that contain the current user
    friend_lists = FriendList.objects.filter(user1=user) or FriendList.objects.filter(user2=user)

    # Extract the associated User objects from each FriendList object
    friends = []
    for friend_list in friend_lists:
        if friend_list.user1 == user:
            friends.append(friend_list.user2)
        else:
            friends.append(friend_list.user1)

    # Pass the list of friends to the template for rendering
    return render(request, 'friends.html', {'friends': friends})

@login_required
def add_friend(request, username):
    friend = get_object_or_404(User, username=username)
    user = request.user
    if user == friend:
        return redirect('user_page', username=username)

    if FriendList.objects.filter(user1=user, user2=friend).exists() or FriendList.objects.filter(user1=friend, user2=user).exists():
        return redirect('user_page', username=username)

    FriendList.objects.create(user1=user, user2=friend)
    return redirect('user_page', username=username)

@login_required
def delete_friend(request, username):
    friend = get_object_or_404(User, username=username)
    user = request.user
    if not FriendList.objects.filter(user1=user, user2=friend).exists():
        # If the friend does not exist, redirect to the user page
        return redirect('user_page', username=username)

    FriendList.objects.filter(user1=user, user2=friend).delete()
    return redirect('user_page', username=username)

# function to update the user's bio 
@login_required
def create_bio(request):
    # check if the request is 'POST'
    if request.method == 'POST':
        form = UserBioForm(request.POST)
        # check for form validation
        if form.is_valid():
            # get the bio and store it 
            bio = form.cleaned_data['bio']
            app_user = AppUser.objects.get_or_create(user=request.user)[0]
            app_user.bio = bio
            # save to db 
            app_user.save()

            messages.success(request, 'Bio has been updated')
            return redirect('home')
        else:
            messages.error(request, 'There was an error updating the bio')
    else:
        form = UserBioForm()
    return render(request, 'create_bio.html', {'form': form})

# function that only works if user is logged in 
@login_required
# function to create post status         
def create_post(request):
    if request.method == 'POST':
        form = PostForm(request.POST)
        if form.is_valid:
            post = form.save(commit=False)
            post.user = request.user.appuser
            post.save()
            messages.success(request, 'Your post has been created!')
            return redirect('home')
        else:
            messages.error(request, 'There was an error creating your post')
    else:
        form = PostForm()
    return render(request, 'create_post.html', {'form': form})

@login_required
def update_post(request, pk):
    post = get_object_or_404(Post, pk=pk)
    if request.method == 'POST':
        form = PostForm(request.POST, instance=post)
        if form.is_valid():
            form.save()
            return redirect('home')
    else:
        form = PostForm(instance=post)
    return render(request, 'update_post.html', {'form': form})

@login_required
def delete_post(request, pk):
    post = get_object_or_404(Post, pk=pk)
    if request.method == 'POST':
        post.delete()
        return redirect('home')
    return render(request, 'delete_post.html', {'post': post})

# function that only works if user is logged in 
@login_required
# function to upload an image 
def create_image(request):
    if request.method == 'POST':
        form = ImageForm(request.POST, request.FILES)
        if form.is_valid:
            img = form.save(commit=False)
            img.user = request.user.appuser
            
            if 'image' in request.FILES:
                img.image = request.FILES['image']

            img.save()

            messages.success(request, 'Your image has been uploaded!')
            return redirect('home')
        else:
            messages.error(request, 'There was an error uploading the image')
    else:
        form = ImageForm()
    return render(request, 'create_image.html', {'form': form})

@login_required
def update_image(request, pk):
    img = get_object_or_404(Image, pk=pk)
    if request.method == 'POST':
        form = ImageForm(request.POST, request.FILES, instance=img)
        if form.is_valid():
            form.save()
            return redirect('home')
    else:
        form = ImageForm(instance=img)
    return render(request, 'update_image.html', {'form': form})

@login_required
def delete_image(request, pk):
    img = get_object_or_404(Image, pk=pk)
    if request.method == 'POST':
        os.remove(os.path.join(settings.MEDIA_ROOT, img.image.path))
        img.delete()
        return redirect('home')
    else:
        form = ImageForm(instance=img)
    return render(request, 'delete_image.html', {'form': form, 'image': img})

@login_required
def chat_view(request, username):
    user = request.user
    receiver = get_object_or_404(User, username=username)

    messages = ChatMessage.objects.filter(
        (Q(sender=user) & Q(receiver=receiver)) | (Q(sender=receiver) & Q(receiver=user))
    ).order_by('timestamp')

    if request.method == 'POST':
        form = ChatMessageForm(request.POST)
        if form.is_valid():
            message_text = form.cleaned_data['message']
            chat_message = ChatMessage.objects.create(
                sender=user, receiver=receiver, message=message_text, timestamp=timezone.now()
            )
            return redirect('chat', username=username)
    else:
        form = ChatMessageForm()

    context = {
        'receiver': receiver,
        'messages': messages,
        'form': form,
    }
    return render(request, 'chat.html', context)

@login_required
def chat_history_view(request):
    user = request.user

    # get distinct participants
    participants = (
        ChatMessage.objects.filter(Q(sender=user) | Q(receiver=user))
        .exclude(sender=user, receiver=user)
        .values('sender', 'receiver')
        .annotate(timestamp=Max('timestamp'))
        .values('sender', 'receiver', 'timestamp')
        .distinct()
    )

    # group participants by their ids
    grouped_participants = {}
    for participant in participants:
        sender_id = participant['sender']
        receiver_id = participant['receiver']

        if sender_id == user.id:
            other_user_id = receiver_id
        else:
            other_user_id = sender_id

        if other_user_id not in grouped_participants:
            grouped_participants[other_user_id] = participant

    # get chat histories for each group
    chat_history = []
    for participant in grouped_participants.values():
        sender_id = participant['sender']
        receiver_id = participant['receiver']

        if sender_id == user.id:
            other_user_id = receiver_id
        else:
            other_user_id = sender_id

        other_user = User.objects.get(id=other_user_id)

        latest_message = ChatMessage.objects.filter(
            (Q(sender=user) & Q(receiver=other_user)) | (Q(sender=other_user) & Q(receiver=user))
        ).order_by('-timestamp').first()

        chat_history.append({
            'participant': other_user,
            'latest_message': latest_message
        })

    context = {
        'chat_history': chat_history
    }
    return render(request, 'chat_history.html', context)
                

# function to register new users
def register(request):
    registered = False

    if request.method == 'POST':
        user_form = UserForm(request.POST)
        profile_form = UserProfileForm(request.POST, request.FILES)

        if user_form.is_valid() and profile_form.is_valid():
            user = user_form.save(commit=False)
            user.set_password(user_form.cleaned_data['password'])
            user.save()

            profile = profile_form.save(commit=False)
            profile.user = user

            if 'profile_pic' in request.FILES:
                profile.profile_pic = request.FILES['profile_pic']

            profile.save()

            registered = True

        else:
            print(user_form.errors, profile_form.errors)

    else:
        user_form = UserForm()
        profile_form = UserProfileForm()
    
    if registered:
        messages.success(request, "Registration was successful. Please login.")
        return redirect('login')

    return render(request, 'register.html', {
        'user_form': user_form,
        'profile_form': profile_form,
        'registered': registered
    })

# function to login as a user
def user_login(request):
    # check if method == POST
    if request.method == 'POST':
        # get user input
        username = request.POST['username']
        password = request.POST['password']

        # check if password and username matches 
        user = authenticate(username=username, password=password)

        # if it checks out
        if user: 
            if user.is_active:
                login(request, user)
                return redirect('home')

            else:
                return HttpResponseRedirect('Your account was disabled!')
        else:
            return HttpResponseRedirect('Invalid Login!')
    
    else:
        return render(request, 'login.html')

# function that only works if user is logged in 
@login_required
# logout function
def user_logout(request):
    logout(request)
    return redirect('home')

