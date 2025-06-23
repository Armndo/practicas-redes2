// Importamos los módulos necesarios
const http = require('http');
const url = require('url');
// Creamos el servidor HTTP
const server = http.createServer((req, res) => {
  // Parseamos la URL y los parámetros de la petición
  const parsedUrl = url.parse(req.url, true);
  const path = parsedUrl.pathname;
  const method = req.method.toUpperCase();
  // Configuramos las cabeceras de la respuesta
  res.setHeader('Content-Type', 'application/json');
  // Definimos las rutas para GET y POST
  if (method === 'GET' && path === '/get') {
    // Ejemplo de respuesta a una petición GET
    const queryParams = parsedUrl.query;
    res.statusCode = 200;
    res.end(JSON.stringify({
      message: 'Petición GET recibida',
      params: queryParams,
    }));
  } else if (method === 'POST' && path === '/post') {
    // Ejemplo de respuesta a una petición POST
    let body = '';
    // Recolectamos los datos del cuerpo de la petición
    req.on('data', chunk => {
      body += chunk.toString();
    });
    req.on('end', () => {
      res.statusCode = 200;
      res.end(JSON.stringify({
        message: 'Petición POST recibida',
        data: JSON.parse(body),
      }));
    });
  } else {
    // Respuesta para rutas no encontradas
    res.statusCode = 404;
    res.end(JSON.stringify({
      message: 'Ruta no encontrada',
    }));
  }
});
// Configuramos el puerto donde va a escuchar el servidor
const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Servidor escuchando en el puerto ${PORT}`);
});