from django.contrib import admin
from .models import *

# Register your models here.

Models = [Post, Image, FriendList, ChatMessage, AppUser]
admin.site.register(Models)