from django.shortcuts import render
from django.http import HttpResponse
# Create your views here.
# 
import socket
import sys
import json

import urllib2

def enviarmensajearduino(mensaje):
	web=""
	
	
	if "getdata" in mensaje:
		#web="http://192.168.1.20/getdata/"
		web="http://192.168.0.20/getdata/"
	if "setdata" in mensaje:
		variableset1=mensaje.split(',')[-2]
		variableset2=mensaje.split(',')[-1]
		web="http://192.168.0.20/setdata/"+variableset1+"-"+variableset2
	userAgent = 'Navegador local'
	headers = { 'User-Agent' : userAgent }
	req = urllib2.Request(web , None, headers)
	response = urllib2.urlopen(req)
	html=response.read()	
	response.close()
	return html


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
	
	web="http://192.168.0.20/getdata/as"
	userAgent = 'NuestroNavegador'
	headers = { 'User-Agent' : userAgent }
	req = urllib2.Request(web , None, headers)
	response = urllib2.urlopen(req)
	html=response.read()
	print html
	response.close()
	return HttpResponse(":))) "+html)

def setdata(request,variable):
	
	web="http://www.google.com/setdata/"+variable
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

def getvalores():
	#import pdb; pdb.set_trace()
	datosrecibidos=enviarmensajearduino("weblocalhost,getdata")

	miboleano={'0':False,'1':True}
	
	valores={}
	valores['Alarma']=miboleano[datosrecibidos[0]]
	valores['Aire 1']=miboleano[datosrecibidos[1]]
	valores['Aire 2']=miboleano[datosrecibidos[2]]
	valores['Radio 1']=miboleano[datosrecibidos[3]]
	valores['Foco']=miboleano[datosrecibidos[4]]
	valores['Arbol']=miboleano[datosrecibidos[5]]
	valores['Puerta']=miboleano[datosrecibidos[6]]
	
	valortemperatura=datosrecibidos.split('-')[-2]
	valorsensor=datosrecibidos.split('-')[-1]
	
	if valorsensor=='1':
		valorsensor=True 
	elif valorsensor=='0':
		valorsensor=False
	sensores={'temperatura':valortemperatura,'alarma':valorsensor}
	return valores,sensores
def dashboard(request):
	template='home/index.html'
	valores,sensores="",{}
	
	valores={'Alarma':False,'Aire 1':False,'Aire 2':False,'Radio 1':False,'Lampara':False,'Foco':False,
		'Arbol':False,'Radio 2':False,'Puerta':False,}
	try:
		#raise "asas"
		
		valores,sensores=getvalores()

	except Exception as ex:
		sensores={}
		sensores['mensajeerror']="No se ha podido conectarse al arduino"
		ctx={'hola':"hola",'valores':valores,'sensores':sensores}
		return render(request,template,ctx)

	else:
		pass
	finally:
		pass
	ctx={'valores':valores,'sensores':sensores,}
	return render(request,template,ctx)



def setvalor(request):
	
	
	nombre =str(request.GET['nombre'])
	if str(request.GET['valor'])=='false':
		valor=False
	else:
		valor=True
	
	try:
		enviarmensajearduino("setdata,"+nombre+','+str(valor))
	except Exception as ex :
		mensaje="Ha ocurrido un error de conexion al server socket"
		payload = {'error':mensaje}
		#sock.close()

		return HttpResponse(json.dumps(payload), content_type='application/json'	)
	else:
		pass
	finally:
		#sock.close()	
		pass
	mensaje="Muy bien muchachito"
	payload = {'mensaje':mensaje,}


	return HttpResponse(json.dumps(payload), content_type='application/json')



def micuenta(request):
	template="home/micuenta.html"
	ctx={'hola':"hola",}
	return render(request,template,ctx)

def inicio(request):
	template="pluto/index.html"
	ctx={'hola':"hola",}
	return render(request,template,ctx)