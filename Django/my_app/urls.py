from django.urls import include, path
from . import views
from .api import *

urlpatterns = [
    path('', views.index, name='home'),

    path('create_post/', views.create_post, name='create_post'),
    path('posts/<int:pk>/', views.update_post, name='update_post'),
    path('posts/<int:pk>/delete/', views.delete_post, name='delete_post'),

    path('create_image/', views.create_image, name='create_image'),
    path('images/<int:pk>/', views.update_image, name='update_image'),
    path('images/<int:pk>/delete', views.delete_image, name='delete_image'),

    path('create_bio/', views.create_bio, name='create_bio'),

    path('search/', views.search_user, name='search_user'),


    path('register/', views.register, name='register'),
    path('login/', views.user_login, name='login'),
    path('logout/', views.user_logout, name='logout'),

    path('friends/list', views.view_friends, name='friend_list'),
    path('add-friend/<str:username>/', views.add_friend, name='add_friend'),
    path('delete-friend/<str:username>/', views.delete_friend, name='delete_friend'),

    path('chat/<str:username>/', views.chat_view, name='chat'),
    path('history/chat', views.chat_history_view, name='chat_history'),

    path('<str:username>/', views.user_page, name='user_page'),

    # APIs
    path('api/users/', UserList.as_view(), name='user-list'),
    path('api/appusers/<int:pk>/', AppUserRetrieveUpdateDestroyView.as_view(), name='user-RUD'),
    path('api/posts/', PostListCreateView.as_view(), name='post-list'),
    path('api/posts/<int:pk>/', PostRetrieveUpdateDestroyView.as_view(), name='post-RUD'),
    path('api/images/', ImageListCreateView.as_view(), name='image-list'),
    path('api/images/<int:pk>/', ImageRetrieveUpdateDestroyView.as_view(), name='image-RUD'),
    path('api/friendlist/<int:pk>/', FriendListView.as_view(), name='friendlist'),
    path('api/chat', ChatMessageListCreateAPIView.as_view(), name='chat-list'),
]