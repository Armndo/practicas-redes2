import pickle 
import requests

# Leer el archivo PDF 
with open('test.pdf', 'rb') as f:
  pdf_data = f.read()
  
# Serializar el contenido del PDF  
serialized_data = pickle.dumps(pdf_data)

# Enviar el contenido serializado al servidor
response = requests.post(
  'http://localhost:5555/prueba_serializacion',
  files= {
    'file': ('documento.pdf', serialized_data)
  }
) 
print(response.text)