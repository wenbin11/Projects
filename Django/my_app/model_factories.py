import factory
from django.contrib.auth.models import User
from .models import AppUser, Post, Image, FriendList, ChatMessage


class UserFactory(factory.django.DjangoModelFactory):
    class Meta:
        model = User

    username = factory.Sequence(lambda n: f"user{n}")
    email = factory.Sequence(lambda n: f"user{n}@example.com")
    password = factory.PostGenerationMethodCall("set_password", "password")


class AppUserFactory(factory.django.DjangoModelFactory):
    class Meta:
        model = AppUser

    user = factory.SubFactory(UserFactory)
    bio = factory.Faker("text", max_nb_chars=1800)
    profile_pic = factory.django.ImageField()


class PostFactory(factory.django.DjangoModelFactory):
    class Meta:
        model = Post

    user = factory.SubFactory(AppUserFactory)
    post_content = factory.Faker("text", max_nb_chars=1800)


class ImageFactory(factory.django.DjangoModelFactory):
    class Meta:
        model = Image

    user = factory.SubFactory(AppUserFactory)
    image = factory.django.ImageField()


class FriendListFactory(factory.django.DjangoModelFactory):
    class Meta:
        model = FriendList

    user1 = factory.SubFactory(UserFactory)
    user2 = factory.SubFactory(UserFactory)


class ChatMessageFactory(factory.django.DjangoModelFactory):
    class Meta:
        model = ChatMessage

    sender = factory.SubFactory(UserFactory)
    receiver = factory.SubFactory(UserFactory)
    message = factory.Faker("text")
    timestamp = factory.Faker("date_time_this_month")