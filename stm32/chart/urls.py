from django.conf.urls import url

from . import views

from django.contrib.staticfiles.urls import staticfiles_urlpatterns

urlpatterns = [
    url(r'^linediagram/$', views.showlinediagram),
    url(r'^test/$', views.test),
    url(r'^add/$', views.add),
]

urlpatterns += staticfiles_urlpatterns()