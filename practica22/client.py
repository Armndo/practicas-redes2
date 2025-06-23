import tkinter as tk
from tkinter import messagebox, filedialog, scrolledtext
import xmlrpc.client
import base64

# Create XML-RPC proxy
proxy = xmlrpc.client.ServerProxy("http://localhost:8000/")

def create_gui():
  root = tk.Tk()
  root.title("Cliente de Gestión de Archivos")
  root.geometry("800x600")

  # Frame for file list and buttons
  frame_left = tk.Frame(root)
  frame_left.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=10, pady=10)

  # Frame for file content display
  frame_right = tk.Frame(root)
  frame_right.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=10, pady=10)

  # File list components
  tk.Label(frame_left, text="Archivos en el servidor:").pack(anchor=tk.W)
  global listbox
  listbox = tk.Listbox(frame_left, width=40, height=20)
  listbox.pack(fill=tk.BOTH, expand=True, pady=5)

  scrollbar = tk.Scrollbar(listbox)
  scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
  listbox.config(yscrollcommand=scrollbar.set)
  scrollbar.config(command=listbox.yview)

  # Buttons for file operations
  button_frame = tk.Frame(frame_left)
  button_frame.pack(fill=tk.X, pady=5)

  tk.Button(button_frame, text="Actualizar lista", command=listar_archivos).pack(side=tk.LEFT, padx=2)
  tk.Button(button_frame, text="Leer archivo", command=leer_archivo).pack(side=tk.LEFT, padx=2)
  tk.Button(button_frame, text="Subir archivo", command=enviar_archivo).pack(side=tk.LEFT, padx=2)
  # tk.Button(button_frame, text="Descargar archivo", command=descargar_archivo).pack(side=tk.LEFT, padx=2)
  # tk.Button(button_frame, text="Eliminar archivo", command=eliminar_archivo).pack(side=tk.LEFT, padx=2)

  # File content display
  tk.Label(frame_right, text="Contenido del archivo:").pack(anchor=tk.W)
  global text_area
  text_area = scrolledtext.ScrolledText(frame_right, width=50, height=25)
  text_area.pack(fill=tk.BOTH, expand=True)

  # Initial file list load
  listar_archivos()

  root.mainloop()

def listar_archivos():
  try:
    archivos = proxy.list_files(".")
    listbox.delete(0, tk.END) # Limpiar la lista
    for archivo in archivos:
      listbox.insert(tk.END, archivo) # Añadir archivo a la lista
  except Exception as e:
    messagebox.showerror("Error", f"No se pudieron listar los archivos: {e}")

def leer_archivo():
  archivo_seleccionado = listbox.get(tk.ACTIVE)
  if archivo_seleccionado:
    try:
      contenido = proxy.read_file(archivo_seleccionado)
      text_area.delete(1.0, tk.END)
      text_area.insert(tk.END, contenido) # Mostrar el contenido del archivo en la GUI
    except Exception as e:
      messagebox.showerror("Error", f"No se pudo leer el archivo: {e}")
  else:
    messagebox.showwarning("Advertencia", "Por favor, selecciona un archivo para leer.")

def enviar_archivo():
  # Seleccionar archivo local
  archivo_seleccionado = filedialog.askopenfilename()
  if archivo_seleccionado:
    try:
      # Leer el archivo y codificarlo en base64
      with open(archivo_seleccionado, "rb") as file:
        archivo_codificado = base64.b64encode(file.read()).decode('utf-8')
        # Obtener solo el nombre del archivo
        nombre_archivo = archivo_seleccionado.split("/")[-1]
        # Enviar el archivo al servidor
        resultado = proxy.upload_file(nombre_archivo, archivo_codificado)
        messagebox.showinfo("Éxito", resultado)
        listar_archivos() # Actualizar lista de archivos en el servidor
    except Exception as e:
      messagebox.showerror("Error", f"No se pudo enviar el archivo: {e}")

if __name__ == "__main__":
  create_gui()