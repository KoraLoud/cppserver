//written by Kora Loudermilk

import java.net.*;
import java.io.*;

public class Client
{
    private Socket socket;
    private DataInputStream in;
    private DataOutputStream out;

    Client(String addr, int port)
    {
        try
        {
            System.out.println("connecting...");
            socket = new Socket(addr, port);
            System.out.println("connected");
            in = new DataInputStream(System.in);

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

        String line = "";
        while (!line.equals("Over"))
        {
            try
            {
                line = System.console().readLine();
                out.writeUTF(line);
            }
            catch (IOException e)
            {
                System.out.println(e);
            }
        }

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
