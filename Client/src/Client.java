//written by Kora Loudermilk

import java.net.*;
import java.io.*;

//TODO:
    //add way to listen for any response that comes in. currently can only listen for data returned after send
public class Client
{
    private Socket socket;
    private DataInputStream in;
    private DataOutputStream out;
    private DataInputStream rescIn;

    Client(String addr, int port)
    {
        try
        {
            System.out.println("connecting...");
            socket = new Socket(addr, port);
            System.out.println("connected");
            in = new DataInputStream(System.in);
            rescIn = new DataInputStream(socket.getInputStream());
            out = new DataOutputStream(socket.getOutputStream());
        }
        catch (UnknownHostException e)
        {
            System.out.println(e);
        }
        catch (IOException e)
        {
            System.out.println(e);
        }
    }

    public String sendData(String data)
    {
        try
        {
            out.writeUTF(data);
            return rescIn.readUTF();
            //System.out.println(rescIn.readUTF());
        }
        catch (IOException e)
        {
            System.out.println(e);
            return "ERROR";
        }
    }

    public void disconnect()
    {
        try
        {
            in.close();
            out.close();
            socket.close();
        }
        catch(IOException e)
        {
            System.out.println(e);
        }
    }
}
