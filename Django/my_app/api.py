from rest_framework.generics import ListCreateAPIView, RetrieveUpdateDestroyAPIView, RetrieveAPIView
from django.db.models import Q

from .models import *
from .serializers import *
from .forms import *


# List all the users and Perform Create as well
class UserList(ListCreateAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer

# Other operations for AppUser
class AppUserRetrieveUpdateDestroyView(RetrieveUpdateDestroyAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer

# List and Create Posts 
class PostListCreateView(ListCreateAPIView):
    queryset = Post.objects.all()
    serializer_class = PostSerializer

# Other operations for Post
class PostRetrieveUpdateDestroyView(RetrieveUpdateDestroyAPIView):
    queryset = Post.objects.all()
    serializer_class = PostSerializer

# List and Create Image
class ImageListCreateView(ListCreateAPIView):
    queryset = Image.objects.all()
    serializer_class = ImageSerializer

# Other operations for Post
class ImageRetrieveUpdateDestroyView(RetrieveUpdateDestroyAPIView):
    queryset = Image.objects.all()
    serializer_class = ImageSerializer

# List and Create Friend List
class FriendListView(RetrieveAPIView):
    queryset = FriendList.objects.all()
    serializer_class = FriendListSerializer
    lookup_field = 'pk'

# List and Create Chat Messages
class ChatMessageListCreateAPIView(ListCreateAPIView):
    serializer_class = ChatMessageSerializer
    
    def get_queryset(self):
        user = self.request.user
        return ChatMessage.objects.filter(Q(sender=user) | Q(receiver=user)).order_by('timestamp')
