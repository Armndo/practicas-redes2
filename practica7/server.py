import cv2, imutils, socket
import numpy as np
import time
import base64

BUFF_SIZE = 65536
server_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server_socket.setsockopt(socket.SOL_SOCKET,socket.SO_RCVBUF,BUFF_SIZE)
host_name = socket.gethostname()
host_ip = '0.0.0.0' # socket.gethostbyname(host_name)
print(host_ip)
port = 5555
socket_address = (host_ip,port)
server_socket.bind(socket_address)
print('Escuchando a:',socket_address)

vid = cv2.VideoCapture(0) # utilizer 0 para usar webcam
fps,st,frames_to_count,cnt = (0,0,10,0)

while True:
  msg,client_addr = server_socket.recvfrom(BUFF_SIZE)
  print('Se tiene conexion de ',client_addr)
  WIDTH=400

  while(vid.isOpened()):
    _, frame = vid.read()
    frame = imutils.resize(frame,width=WIDTH)
    encoded,buffer = cv2.imencode('.jpg',frame,[cv2.IMWRITE_JPEG_QUALITY,80])
    message = base64.b64encode(buffer)
    server_socket.sendto(message,client_addr)
    frame = cv2.putText(frame,'FPS:'+str(fps),(10,40),cv2.FONT_HERSHEY_SIMPLEX,0.7,(0,0,255),2)
    cv2.imshow('Transmitiendo video',frame)
    key = cv2.waitKey(1) & 0xFF

    if key == ord('q'):
      server_socket.close()
      break

    if cnt == frames_to_count:
      try:
        fps = round(frames_to_count/(time.time()-st))
        st=time.time()
        cnt=0
      except:
        pass
    cnt+=1
