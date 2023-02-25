from rest_framework import serializers
from .models import *

class PostSerializer(serializers.ModelSerializer):
    post_id = serializers.IntegerField(source='id')
    class Meta:
        model = Post
        fields = ('post_id', 'user', 'post_content')

    def to_representation(self, instance):
        rep = super().to_representation(instance)
        rep['user'] = instance.user.user.username
        return rep

class ImageSerializer(serializers.ModelSerializer):
    image_id = serializers.IntegerField(source='id')
    class Meta:
        model = Image
        fields = ('image_id', 'user', 'image')
    
    def to_representation(self, instance):
        rep = super().to_representation(instance)
        rep['user'] = instance.user.user.username
        return rep

class AppUserSerializer(serializers.ModelSerializer):
    class Meta:
        model = AppUser
        fields = ('bio', 'profile_pic')

class UserSerializer(serializers.ModelSerializer):
    appuser = AppUserSerializer(required=False)

    class Meta:
        model = User
        fields = ('username', 'email', 'password', 'appuser')
        extra_kwargs = {'password': {'write_only': True}}

    def create(self, validated_data):
        appuser_data = validated_data.pop('appuser', None)
        password = validated_data.pop('password')
        user = User.objects.create(**validated_data)
        user.set_password(password)
        user.save()
        if appuser_data:
            AppUser.objects.create(user=user, **appuser_data)
        return user

    def update(self, instance, validated_data):
        appuser_data = validated_data.pop('appuser', None)
        if appuser_data:
            appuser_serializer = AppUserSerializer(instance.appuser, data=appuser_data)
            appuser_serializer.is_valid(raise_exception=True)
            appuser_serializer.save()
        return super().update(instance, validated_data)

    class Meta:
        model = User
        fields = ('id', 'username', 'email', 'password', 'appuser')

class FriendListSerializer(serializers.ModelSerializer):
    user1 = UserSerializer()
    user2 = UserSerializer()

    class Meta:
        model = FriendList
        fields = ['user1', 'user2']
    
    def create(self, validated_data):
        user1_data = validated_data.pop('user1')
        user2_data = validated_data.pop('user2')

        # Get the user objects based on the input data
        user1 = User.objects.get(id=user1_data['id'])
        user2 = User.objects.get(id=user2_data['id'])

        # Create the FriendList object with the user objects
        friend_list = FriendList.objects.create(user1=user1, user2=user2)

        return friend_list


class ChatMessageSerializer(serializers.ModelSerializer):
    sender_username = serializers.ReadOnlyField(source='sender.username')
    receiver_username = serializers.ReadOnlyField(source='receiver.username')
    
    class Meta:
        model = ChatMessage
        fields = ['sender', 'sender_username', 'receiver', 'receiver_username', 'message', 'timestamp']