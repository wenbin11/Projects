# Generated by Django 3.2.18 on 2023-02-21 04:34

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('my_app', '0003_alter_image_image'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='image',
            name='image_id',
        ),
        migrations.RemoveField(
            model_name='post',
            name='post_id',
        ),
    ]
