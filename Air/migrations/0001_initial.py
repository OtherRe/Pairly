# Generated by Django 2.1.3 on 2018-12-02 11:36

import Air.devices.models
from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion
import djongo.models.fields


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
    ]

    operations = [
        migrations.CreateModel(
            name='Device',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('public_key', models.CharField(max_length=30)),
                ('data_type', models.CharField(max_length=5)),
                ('location', djongo.models.fields.EmbeddedModelField(model_container=Air.devices.models.Location, null=True)),
                ('data_points', djongo.models.fields.ArrayModelField(model_container=Air.devices.models.Data)),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='devices', to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]