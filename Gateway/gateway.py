# -*- coding: cp1252 -*-
from socket import *
import mysql.connector

con = mysql.connector.connect(user='root', password='', host='localhost', database='meshdb')
cursor = con.cursor()


def inserirBanco(cursor, lista):
    cursor.execute("""INSERT INTO dadosCapturados (id, nodeId, rssi, ch, addr, ssid) VALUES (null, '%s', '%s','%s','%s','%s')""" % (
    lista[0], lista[1], lista[2], lista[3], lista[4]))
    con.commit()


serverPort = 12000
# Cria o Socket TCP (SOCK_STREAM) para rede IPv4 (AF_INET)
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))

# Socket fica ouvindo conexoes. O valor 1 indica que uma conexao pode ficar na fila
serverSocket.listen(1)

print("Servidor pronto para receber mensagens. Digite Ctrl+C para terminar.")

while 1:
    try:
        # Cria um socket para tratar a conexao do cliente
        connectionSocket, addr = serverSocket.accept()

        dadosRecebidos = connectionSocket.recv(1024)
        captura = dadosRecebidos.split(" ")

        listaCaptura = captura[0].split("\n")
        print("********* Dados Inseridos no BD *********")
        for texto in listaCaptura:
            lista = texto.split(",")
            if len(lista) == 5:
                inserirBanco(cursor, lista)
            print(texto + "\n")

        connectionSocket.send("Gateway recebeu os dados com sucesso!")

        connectionSocket.close()

    except (KeyboardInterrupt, SystemExit):
        break

serverSocket.close()
