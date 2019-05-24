from django.conf.urls import url

from . import views

from django.contrib.staticfiles.urls import staticfiles_urlpatterns

urlpatterns = [
    url(r'^linediagram/$', views.showlinediagram),
    url(r'^cpu/$', views.test),
    url(r'^tem/$', views.test_tem),
    url(r'^hum/$', views.test_hum),
    url(r'^add_cpu/$', views.add_cpu),
    url(r'^add_tem/$', views.add_tem),
    url(r'^add_hum/$', views.add_hum),
]

urlpatterns += staticfiles_urlpatterns()