from rest_framework.test import APITestCase
from rest_framework import status
from django.urls import reverse
from .model_factories import *
from .models import *
from .serializers import *

from django.contrib.auth.models import User
from django.core.files.uploadedfile import InMemoryUploadedFile
from rest_framework.test import APITestCase
from rest_framework import status
import PIL.Image as PILImage
from io import BytesIO


# test cases to test for PostSerializers
class PostSerializerTestCase(APITestCase):
    def setUp(self):
        self.user = User.objects.create_user(username='testuser', password='testpass')
        self.app_user = AppUser.objects.create(user=self.user, bio='Test bio', profile_pic='test_pic.jpg')
        self.post = Post.objects.create(post_content='Test post', user=self.app_user)
        self.client.force_authenticate(user=self.user)
        self.serializer = PostSerializer(instance=self.post)
        self.delete_url = reverse('delete_post', kwargs={'pk': self.user.id})
    
    # emptying the values and resetting indices
    def tearDown(self):
        Post.objects.all().delete()
        User.objects.all().delete()
        AppUser.objects.all().delete()

        PostFactory.reset_sequence(0)
        UserFactory.reset_sequence(0)
        AppUserFactory.reset_sequence(0)

    # test for serializers containing the correct keys 
    def test_contains_expected_fields(self):
        data = self.serializer.data
        self.assertEqual(set(data.keys()), {'post_content', 'user'})
    
    # test whether the data has been parsed correctly
    def test_post_content_field_content(self):
        data = self.serializer.data
        self.assertEqual(data['post_content'], self.post.post_content)
    
    # test whether the user.id has been parsed correctly
    def test_user_field_content(self):
        data = self.serializer.data
        self.assertEqual(data['user'], self.post.user.id)
    
    # test to perform creation of a successful post (302 as user is redirected to homepage after creating post)
    def test_create_valid_post(self):
        data = {'post_content': 'New test post content', 'user': self.post.user.id}
        response = self.client.post('/create_post/', data, format='json')
        self.assertEqual(response.status_code, status.HTTP_302_FOUND)
    
    # test to perform deletion of a post (302 as user is redirected to homepage after creating post)
    def test_delete_post(self):
        response = self.client.delete(self.delete_url, format='json')
        self.assertEqual(response.status_code, status.HTTP_302_FOUND)

# test cases to test for ImageSerializers
class ImageSerializerTestCase(APITestCase):
    def setUp(self):
        self.user = User.objects.create_user(
            username='testuser', email='test@example.com', password='testpassword')
        self.app_user = AppUser.objects.create(
            user=self.user, bio='Test bio', profile_pic='profile_pic/default.png')

        self.client.login(username=self.user.username, password=self.user.password)

        # create a temporary file for testing image upload
        image = PILImage.new('RGB', (100, 100), color='red')
        tmp_file = BytesIO()
        image.save(tmp_file, 'jpeg')
        tmp_file.seek(0)

        # create an InMemoryUploadedFile object from the BytesIO buffer
        image_file = InMemoryUploadedFile(tmp_file, 'ImageField', 'test.jpg', 'image/jpeg', len(tmp_file.getvalue()), None)

        self.image = Image.objects.create(
            user=self.app_user, image=image_file)
        
        self.serializer = ImageSerializer(instance=self.image)

        # create a valid serialized representation of the image
        self.valid_payload = {
            'image': self.image.image,
            'user': self.app_user
        }

        # create an invalid serialized representation of the image
        self.invalid_payload = {
            'user': 11,
            'image': 'invalid_image'
        }

    # emptying the values and resetting indices
    def tearDown(self):
        Image.objects.all().delete()
        User.objects.all().delete()
        AppUser.objects.all().delete()

        ImageFactory.reset_sequence(0)
        UserFactory.reset_sequence(0)
        AppUserFactory.reset_sequence(0)
    
    # test for serializers containing the correct keys 
    def test_contains_expected_fields(self):
        data = self.serializer.data
        self.assertEqual(set(data.keys()), {'user', 'image'})
    
    # test whether the image has been parsed correctly
    def test_post_content_field_content(self):
        data = self.serializer.data
        self.assertEqual(data['image'], self.image.image.url)
    
    # test whether the user.id has been parsed correctly
    def test_user_field_content(self):
        data = self.serializer.data
        self.assertEqual(data['user'], self.image.user.id)

# test cases to test for UserSerializers
class UserSerializerTestCase(APITestCase):
    def setUp(self):
        self.user = User.objects.create_user(
            username='testuser', email='test@example.com', password='testpassword')
        self.app_user = AppUser.objects.create(
            user=self.user, bio='Test bio', profile_pic='profile_pic/default.png')
        
        # create a temporary file for testing image upload
        image = PILImage.new('RGB', (100, 100), color='red')
        tmp_file = BytesIO()
        image.save(tmp_file, 'jpeg')
        tmp_file.seek(0)

        # create an InMemoryUploadedFile object from the BytesIO buffer
        image_file = InMemoryUploadedFile(tmp_file, 'ImageField', 'test.jpg', 'image/jpeg', len(tmp_file.getvalue()), None)

        self.image = Image.objects.create(
            user=self.app_user, image=image_file)

        # create a valid serialized representation of the user
        self.valid_payload = {
            'username': 'newusername',
            'email': 'newemail@example.com',
            'password': 'newpassword',
            'appuser': {
                'bio': 'newbio',
                'profile_pic': self.image.image,
            }
        }


        # create an invalid serialized representation of the user
        self.invalid_payload = {
            'username': '',
            'email': 'invalid_email',
            'password': '',
            'appuser': {
                'bio': '',
                'profile_pic': 'profile_pic',
            }
        }

    # check against serializer to make sure data is valid
    def test_user_serializer_valid_data(self):
        serializer = UserSerializer(data=self.valid_payload)
        self.assertTrue(serializer.is_valid(), serializer.errors)
    
    # check for errors in keys
    def test_user_serializer_invalid_data(self):
        serializer = UserSerializer(data=self.invalid_payload)
        self.assertFalse(serializer.is_valid())
        self.assertEqual(set(serializer.errors.keys()), {'username', 'email', 'password', 'appuser'})
    
    # check for successful creation with correct data
    def test_user_serializer_create(self):
        serializer = UserSerializer(data=self.valid_payload)
        self.assertTrue(serializer.is_valid())
        user = serializer.save()

        # check the User object
        self.assertEqual(User.objects.count(), 2)
        self.assertEqual(user.username, 'newusername')
        self.assertEqual(user.email, 'newemail@example.com')
        # as the password will be hashed after creating, we need to reset the password
        user.set_password('newpassword')
        self.assertTrue(user.check_password('newpassword'))

        # check the AppUser object
        appuser = user.appuser
        self.assertEqual(AppUser.objects.count(), 2)
        self.assertEqual(appuser.bio, 'newbio')
        self.assertEqual(appuser.profile_pic, self.image.image)
    
# test cases for friendList serializers
class FriendListAPITestCase(APITestCase):
    def setUp(self):
        self.user1 = User.objects.create_user(
            username='user1', email='user1@example.com', password='password1'
        )
        self.user2 = User.objects.create_user(
            username='user2', email='user2@example.com', password='password2'
        )
        self.user3 = User.objects.create_user(
            username='testuser3', email='testuser3@example.com', password='testpass'
        )
        self.friend_list = FriendList.objects.create(user1=self.user1, user2=self.user2)
    
    def tearDown(self) -> None:
        User.objects.all().delete()
        FriendList.objects.all().delete()

        UserFactory.reset_sequence(0)
        FriendListFactory.reset_sequence(0)
    
    # retrieve friend lists
    def test_get_all_friend_lists(self):
        # create users
        user5 = User.objects.create_user(username='user5', password='user5')
        user6 = User.objects.create_user(username='user6', password='user6')
        user7 = User.objects.create_user(username='user7', password='user7')

        # create friend lists
        FriendList.objects.create(user1=user5, user2=user6)
        FriendList.objects.create(user1=user5, user2=user7)

        # authenticate as user1
        self.client.login(username=user5.username, password='user5')

        # make GET request to friends_list endpoint
        response = self.client.get(reverse('friend_list'))

        # check if the response works
        self.assertEqual(response.status_code, status.HTTP_200_OK)

        # check if friend_list contains user6 and user7
        self.assertCountEqual(response.context['friends'], [user6, user7])

        # check if correct template is used
        self.assertTemplateUsed(response, 'friends.html')

    
    # add friend test cases
    def test_add_friend(self):
        url = reverse('add_friend', kwargs={'username': self.user3.username})
        self.client.login(username=self.user1.username, password='password1')
        response = self.client.post(url)
        # check for return a redirect
        self.assertEqual(response.status_code, status.HTTP_302_FOUND)
        # check if friend is created 
        self.assertTrue(FriendList.objects.filter(user1=self.user1, user2=self.user3).exists())
    
    # delete friend test cases
    def test_delete_friend(self):
        url = reverse('delete_friend', kwargs={'username': self.user2.username})
        self.client.login(username=self.user1.username, password='password1')
        response = self.client.delete(url)
        # check for return a redirect
        self.assertEqual(response.status_code, status.HTTP_302_FOUND)
        # check if friend has been deleted
        self.assertFalse(FriendList.objects.filter(user1=self.user1, user2=self.user2).exists())

# test cases for chat serializers
class ChatMessageAPITestCase(APITestCase):
    def setUp(self):
        self.user1 = User.objects.create_user(username='user1', password='password123')
        self.user2 = User.objects.create_user(username='user2', password='password123')
        self.chat_message = ChatMessage.objects.create(sender=self.user1, receiver=self.user2, message='Hello World')
        self.valid_payload = {
            'sender': self.user1.id,
            'receiver': self.user2.id,
            'message': 'New message'
        }
        self.invalid_payload = {
            'sender': self.user1.id,
            'receiver': self.user2.id,
        }

    def tearDown(self) -> None:
        User.objects.all().delete()
        ChatMessage.objects.all().delete()

        UserFactory.reset_sequence(0)
        ChatMessageFactory.reset_sequence(0)
    
    def test_get_all_chat_messages(self):
        # authenticate as user1
        self.client.login(username=self.user1.username, password='password123')
        url = reverse('chat', kwargs={'username': self.user2.username})
        response = self.client.get(url)
        chat_messages = ChatMessage.objects.all()
        serializer = ChatMessageSerializer(chat_messages, many=True)
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(ChatMessage.objects.count(), 1)
    
    def test_send_message(self):
        # authenticate as user1
        self.client.login(username=self.user1.username, password='password123')
        url = reverse('chat', kwargs={'username': self.user2.username})

        data = {'sender': self.user1, 'receiver': self.user2, 'message': 'New Message'}
        response = self.client.post(url, data)

        self.assertEqual(response.status_code, status.HTTP_302_FOUND)
        self.assertEqual(ChatMessage.objects.count(), 2)
        self.assertQuerysetEqual(ChatMessage.objects.filter(message='New Message'), ['<ChatMessage: New Message>'])