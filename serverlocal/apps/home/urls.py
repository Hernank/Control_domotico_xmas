from django.conf.urls import patterns, include, url
from django.contrib import admin
def miurl(direccion):
    obj=direccion.split('/')
    return url((r'^'+direccion+'$'), ('apps.home.views.'+obj[-2]),name=obj[-2])

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'micro.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    miurl('entradas/'),
    miurl('setdata/(.+)'),
    miurl('getdata/'),
    miurl('setsocket/(.+)'),
    
    
)
