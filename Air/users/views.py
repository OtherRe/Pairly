from django.contrib.auth import login, authenticate
from django.contrib.auth.forms import UserCreationForm
from django.shortcuts import render, redirect
from . import forms

# Create your views here.


def signup(request):
    if request.method == 'POST':
        form = forms.SignUpForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            user = authenticate(username=username, password=raw_password)
            login(request, user)
            return render(request, 'index/home.html', {'message':'U have been succesfuly singed up'})
    else:
        form = forms.SignUpForm()
    return render(request, 'users/signup.html', {'form': form})

# class UserProfile()

def profile(request):
    if request.user.is_authenticated:
        return render(request, 'users/profile.html')
    else:
        return render(request,'users/login.html', {'errors': ['Please log in if You want to see your profile']})
