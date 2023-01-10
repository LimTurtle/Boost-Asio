using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System;
using UnityEngine.UIElements;

public class Network_Manager : MonoBehaviour
{
    private TcpClient socketConnection;
    private Thread clientReceiveThread;
    // Start is called before the first frame update
    void Start()
    {
        ConnectToTcpServer();
    }

    private void ConnectToTcpServer()
    {
        try
        {
            clientReceiveThread = new Thread(new ThreadStart(ListenForData));
            clientReceiveThread.IsBackground = true;
            clientReceiveThread.Start();
            
        }
        catch (Exception e)
        {
            Debug.Log("On client connect exception " + e);
        }
    }

    private void ListenForData()
    {
        try
        {
            socketConnection = new TcpClient("127.0.0.1", 31400);
            Debug.Log("서버에 연결됨");
            Byte[] bytes = new Byte[128];
            while (true)
            {
                using (NetworkStream stream = socketConnection.GetStream()) 
                {
                    int length;
                    while ((length = stream.Read(bytes, 0, bytes.Length)) != 0) 
                    {
                        var incommingData = new byte[length];
                        Array.Copy(bytes, 0, incommingData, 0, length);
                        string serverMessage = Encoding.ASCII.GetString(incommingData);
                        Debug.Log("서버로부터 받은 메시지: " + serverMessage);
                    }
                }
            }
        }
        catch(SocketException se)
        {
            Debug.Log("Exception: " + se);
        }
    }

    [ContextMenu("Send Message")]
    private void SendMessage()
    {
        Byte[] buffer = System.Text.Encoding.UTF8.GetBytes("Hello World");
        if (socketConnection == null)
        {
            return;
        }
        try
        {
            NetworkStream stream = socketConnection.GetStream();
            if (stream.CanWrite)
            {
                stream.Write(buffer, 0, buffer.Length);
            }

        }
        catch (SocketException socketException)
        {
            Debug.Log("Socket Exception: " + socketException);
        }
    }

}
