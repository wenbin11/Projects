from django.db import models
from django.contrib.auth.models import User


# Create your models here.
class AppUser(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE, blank=False, null=False)
    bio = models.CharField(max_length=1800, null=True, blank=True)
    profile_pic = models.ImageField(upload_to='profile_pic/', blank=False)
    friends = models.ManyToManyField('self', blank=True)

class Post(models.Model):
    post_content = models.CharField(max_length=1800)
    user = models.ForeignKey(AppUser, on_delete=models.CASCADE, null=False, blank=False, related_name='post')

class Image(models.Model):
    image = models.FileField(upload_to='images/', blank=False)
    user = models.ForeignKey(AppUser, on_delete=models.CASCADE, null=False, blank=False, related_name='image')

class FriendList(models.Model):
    user1 = models.ForeignKey(User, on_delete=models.CASCADE, related_name='user1')
    user2 = models.ForeignKey(User, on_delete=models.CASCADE, related_name='user2')

    class Meta:
        unique_together = ('user1', 'user2')

class ChatMessage(models.Model):
    sender = models.ForeignKey(User, related_name='sender', on_delete=models.CASCADE)
    receiver = models.ForeignKey(User, related_name='receiver', on_delete=models.CASCADE)
    message = models.TextField()
    timestamp = models.DateTimeField(auto_now_add=True)

    class Meta:
        ordering = ('timestamp',)

    def __str__(self):
        return self.message