

class Main {
    public static void main(String[] args)
    {
        Client client = new Client("127.0.0.1", 3000);
        String response = client.sendData("fuck me PLEASE god damn");
        System.out.println(response);
        client.disconnect();
    }
}