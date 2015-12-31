from django.shortcuts import render
from django.http import HttpResponse
# Create your views here.
# 
import socket
import sys


import urllib2



def enviarmensaje(mensaje):
	HOST, PORT = "127.0.0.1", 8000
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect((HOST, PORT))
	sock.sendall(mensaje)
	
	print "enviando mensaje"
	
	return sock


def entradas(request):
	 return HttpResponse('LED=ON')


def getdata(request):
	#import pdb; pdb.set_trace()
	web="http://192.168.1.20/getdata/as"
	userAgent = 'NuestroNavegador'
	headers = { 'User-Agent' : userAgent }
	req = urllib2.Request(web , None, headers)
	response = urllib2.urlopen(req)
	html=response.read()
	print html
	response.close()
	return HttpResponse(":))) "+html)

def setdata(request,variable):
	#import pdb; pdb.set_trace()
	web="http://192.168.1.20/setdata/"+variable
	userAgent = 'Navegador local jhr'
	headers = { 'User-Agent' : userAgent }
	req = urllib2.Request(web , None, headers)
	response = urllib2.urlopen(req)
	html=response.read()
	print html
	response.close()

	return HttpResponse(":)) "+html)


def setsocket(request,variable):
	midata="localserver,"+variable
		# Connect to server and send data
	sock=enviarmensaje( midata)
	received = sock.recv(1024)
	sock.close()

	return HttpResponse("jajaj "+received)