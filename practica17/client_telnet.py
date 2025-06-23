import telnetlib

# Configuración del cliente
HOST = '127.0.0.1'  # Reemplaza con la IP de tu servidor Ubuntu
PORT = 5555         # Debe coincidir con el puerto configurado en el servidor

# Conectar al servidor
tn = telnetlib.Telnet(HOST, PORT)

# Recibir el mensaje de bienvenida (leer hasta "Login: ")
welcome_msg = tn.read_until(b"Login: ").decode('utf-8')
print(welcome_msg, end='')

# Enviar nombre de usuario
username = input().strip()
tn.write(username.encode('utf-8') + b"\n")

# Recibir la solicitud de contraseña (leer hasta "Password: ")
password_prompt = tn.read_until(b"Password: ").decode('utf-8')
print(password_prompt, end='')

# Enviar la contraseña
password = input().strip()
tn.write(password.encode('utf-8') + b"\n")

# Recibir el resultado de la autenticación
auth_result = tn.read_all().decode('utf-8')
print(auth_result)

tn.close()