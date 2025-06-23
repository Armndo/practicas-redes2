
import dgram from "dgram";
import postgres from "postgres";

try {

  const psql = postgres({
    host: "127.0.0.1",
    port: 5432,
    database: "redes2",
    username: "armando",
    password: "root",
  });

  // console.log((await psql`select * from blacklist`).length)

  // Crear un servidor UDP para escuchar las consultas DNS
  const server = dgram.createSocket('udp4');

  // Función para verificar si un dominio está en la lista negra
  async function isBlacklisted(domain, callback) {
    console.log(`Consultando la lista negra para el dominio: ${domain}`);

    try {
      const res = await psql`
        SELECT * FROM blacklist WHERE domain = '${domain}'
      `

      console.log('Resultados de la consulta:', res);
      callback(res.length > 0);
    } catch (err) {
      console.error('Error al consultar la base de datos:', err);
    }
  }

  // Función para manejar las consultas DNS
  server.on('message', (msg, rinfo) => {
    console.log('Consulta recibida:', msg);
    const domain = parseDomainFromQuery(msg);
    console.log('Dominio extraído:', domain);
    if (!domain) {
      console.log('Consulta DNS no válida');
      return;
    }
    isBlacklisted(domain, (blacklisted) => {
      if (blacklisted) {
        console.log(`Dominio bloqueado: ${domain}`);
        sendFakeResponse(msg, rinfo, server);
      } else {
        console.log(`Dominio no bloqueado: ${domain}`);
        forwardQueryToDNS(msg, rinfo, server);
      }
    });
  });

  // Función para analizar el dominio de la consulta DNS
  function parseDomainFromQuery(msg) {
    const question = msg.slice(12);
    let domain = '';
    let i = 0;

    while (i < question.length) {
      const len = question[i];

      if (len === 0) break;

      domain += question.slice(i + 1, i + 1 + len).toString('utf8') + '.';
      i += len + 1;
    }

    return domain.slice(0, -1);
  }

  // Función para enviar una respuesta falsa (IP 0.0.0.0)
  function sendFakeResponse(msg, rinfo, server) {
    const response = Buffer.alloc(msg.length);
    msg.copy(response);
    response[2] = 0x81; // Respuesta con error (NXDOMAIN)
    response[3] = 0x83;
    server.send(response, 0, response.length, rinfo.port, rinfo.address, (err) => {
      if (err) console.error('Error al enviar la respuesta falsa:', err);
    });
  }

  // Función para reenviar la consulta DNS al resolver real
  function forwardQueryToDNS(msg, rinfo, server) {
    const client = dgram.createSocket('udp4');
    client.on('message', (response) => {
      server.send(response, 0, response.length, rinfo.port, rinfo.address,
        (err) => {
          if (err) console.error('Error al enviar la respuesta del DNS real:',
            err);
        });
      client.close();
    });
    client.send(msg, 0, msg.length, 53, '8.8.8.8', (err) => {
      if (err) console.error('Error al enviar la consulta al DNS real:', err);
    });
  }

  // Iniciar el servidor DNS Proxy
  server.bind(
    5354,
    '127.0.0.1',
    () => {
      console.log('Servidor DNS Proxy escuchando en el puerto 5354');
    }
  );
} catch (e) {
  console.log(e);
}