

class Main {
    public static void main(String[] args)
    {
        Client client = new Client("127.0.0.1", 3000);
        String response = client.sendData("test123");
        System.out.println(response);
        if(response.equals("test123"))
        {
            System.out.println(client.sendData("data send successful!"));
        }

        client.disconnect();
    }
}