import socket
import sys

import urllib2

data = " ".join(sys.argv[1:])

# Create a socket (SOCK_STREAM means a TCP socket)
#sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
def enviarmensaje(sock,mensaje):
	HOST, PORT = "127.0.0.1", 8000
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect((HOST, PORT))
	sock.sendall(mensaje)	
	print "mensaje enviado"
	#sock.sendall(mensaje)
	return sock

def enviarmensajearduino(mensaje):
	web=""
	#import pdb; pdb.set_trace()
	if "getdata" in mensaje:
		web="http://192.168.0.20/getdata/as"
	if "setdata" in mensaje:
		variableset1=mensaje.split(',')[-2]
		variableset2=mensaje.split(',')[-1]
		web="http://192.168.0.20/setdata/"+variableset1+"-"+variableset2
	userAgent = 'NuestroNavegador'
	headers = { 'User-Agent' : userAgent }
	req = urllib2.Request(web , None, headers)
	response = urllib2.urlopen(req)
	html=response.read()
	print html+">>>"
	response.close()
	return html

sock=""
while True:	    
	try:
		#midata=raw_input("Ingrese mensaje")
		# Connect to server and send data
		sock=enviarmensaje(sock, "localclient,socket_abierto")
		
		# Receive data from the server and shut down
		print "recibir mensaje:"

		received = sock.recv(1024)
		
		print received
		##import pdb; pdb.set_trace()
		isgetdata="getdata" in received 
		issetdata="setdata" in received


		if isgetdata:
			sock.close
			nueva=received.split(',')			
			arduinoserver=enviarmensajearduino(nueva[-2]+"/"+nueva[-1])
			sucess=True 
			if sucess:
				enviarmensaje(sock, ("localclient,success,"+arduinoserver))
			else:
				enviarmensaje(sock, "localclient,error,"+arduinoserver)
			print "recibir mensaje"
			received = sock.recv(1024)
		if issetdata:

			sock.close

			sucess=True 
			if sucess:
				enviarmensaje(sock, "localclient,success,"+arduinoserver)
			else:
				enviarmensaje(sock, "localclient,error,"+arduinoserver)
			print "recibir mensaje"
			received = sock.recv(1024)

		if "cambio_nuevo" in received:
			sock.close
			print "consultar a la pagina principal http"
			print "ejecuto nueva orden"
			sucess=True 
			##import pdb; pdb.set_trace()
			if sucess:
				enviarmensaje(sock, "localclient,success,")
			else:
				enviarmensaje(sock, "localclient,error,")
			print "recibir mensaje"
			received = sock.recv(1024)
	except Exception as ex:
		##import pdb; pdb.set_trace()
		sock.close()
		print "me desconecte(error no controlado)"+ex.message
	else:
		pass
	finally:
		print ""
		sock.close()
		print "me desconecte"
	