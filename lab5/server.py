from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import os

hostname = "localhost"
port = 1111

safe_path = os.path.realpath(".")

class Server(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        path = self.path[1:]
        print(path)
        if path == "":
            path = "index.html"

        try:
            with open(f"web-pages/{path}", "rb") as f:
                self.wfile.write(f.read())
        except FileNotFoundError:
            self.send_error(404, "Nie znaleziono takiego pliku!")


if __name__ == "__main__":        
    server = HTTPServer((hostname, port), Server)
    print(f"Otworz stronę: http://{hostname}:{port}")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass

    server.server_close()