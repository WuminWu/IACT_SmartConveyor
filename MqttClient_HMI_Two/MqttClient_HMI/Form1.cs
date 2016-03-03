using System;
using System.Threading;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;    //use this namespace for sockets
using System.Net;            //for ip addressing
using System.IO;             //for streaming io

// Add Mqtt library
using uPLibrary.Networking.M2Mqtt;

namespace MqttClient_HMI
{
    public partial class Form1 : Form
    {
        // variable of Mqtt
        const string ipAddress = "127.0.0.1";                   //Port is defined as 1883
        const string TOPIC_One = "IAC DEMO HMI";
        const string TOPIC_Two = "IAC DEMO Two";
        const string TOPIC_MC = "IAC DEMO MC";
        const string SUBSCRIBE_TOPIC_SEQ01_INIT = "SCC->MC:INIT";
        const string PUBLISH_MC_CONV1 = "192.168.1.12";
        const string PUBLISH_MC_CONV2 = "192.168.1.13";
        const string PUBLISH_MC_ELEV2 = "192.168.1.14";
        const string PAYLOAD_respOK = "OK";
        const string PAYLOAD_respError = "Error";
        static MqttClient client = new MqttClient(IPAddress.Parse(ipAddress));

        public Form1()
        {
            InitializeComponent();
        }

        // Mqtt Protocol Function
        public void creatMqttClient()
        {
            //register function
            client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;     // Define what function to call when a message arrives
            client.MqttMsgSubscribed += client_MqttMsgSubscribed;               // Define what function to call when a subscription is acknowledged
            client.MqttMsgPublished += client_MqttMsgPublished;                 // Define what function to call when a message is published

            string clientID = "HMI_Two";
            byte connection = client.Connect(clientID);

            // Wumin : if "RetainedOne" subscribe here and you will get msg twice
            ushort subscribe = client.Subscribe(new string[] { SUBSCRIBE_TOPIC_SEQ01_INIT }, new byte[] { 0 });
            Console.WriteLine("subscribe = " + subscribe);
        }


        //If you are using QoS Level 1 or 2 to publish a message on a specified topic, you can also register to MqttMsgPublished event that will be raised when the message will be delivered (exactly once) to all subscribers on the topic
        private static void client_MqttMsgPublished(object sender, uPLibrary.Networking.M2Mqtt.Messages.MqttMsgPublishedEventArgs e)
        {
            Console.WriteLine("Message " + e.MessageId + " has been sent.\n");
        }

        // Handle subscription acknowledgements
        private static void client_MqttMsgSubscribed(object sender, uPLibrary.Networking.M2Mqtt.Messages.MqttMsgSubscribedEventArgs e)
        {
            Console.WriteLine("Subscribed!");
        }

        // Handle incoming messages
        private static void client_MqttMsgPublishReceived(object sender, uPLibrary.Networking.M2Mqtt.Messages.MqttMsgPublishEventArgs e)
        {
            string publishReceived = Encoding.UTF8.GetString(e.Message);
            Console.WriteLine("Message received: " + publishReceived);
/*
            Console.WriteLine("byte[0] = {0}", e.Message[0]);
            Console.WriteLine("byte[1] = {0}", e.Message[1]);
            Console.WriteLine("byte[2] = {0}", e.Message[2]);
            Console.WriteLine("byte[3] = {0}", e.Message[3]);
*/
            switch (publishReceived)
            {
                case "I am Janet":
                case "I am Sky":
                    Console.WriteLine("HMI Team got Msg\n");
                    break;

                case "I am BZ" :
                case "I am Ben":
                    Console.WriteLine("SCC Team got Msg\n");
                    break;

                default:
                    //Console.WriteLine("publishReceived has no event");
                    break;
            }
        }

        private void button_Connect(object sender, EventArgs e)
        {
            creatMqttClient();
        }

        private void button_Publish_Self(object sender, EventArgs e)
        {
            ushort publishAtoB = client.Publish(TOPIC_One, Encoding.UTF8.GetBytes("I am Janet"));
            Console.WriteLine("publish = "+publishAtoB);
        }

        private void button_PosOne_Motor_On(object sender, EventArgs e)
        {
            // Position Motor 1 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x84, (byte)0x11, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosTwo_Motor_On(object sender, EventArgs e)
        {
            // Position Motor 2 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x44, (byte)0x11, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosThree_Motor_On(object sender, EventArgs e)
        {
            // Position Motor 3 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x24, (byte)0x11, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosFour_Motor_On(object sender, EventArgs e)
        {
            // Position Motor 4 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x14, (byte)0x11, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosOne_Cylinder_On(object sender, EventArgs e)
        {
            // Position Cylinder 1 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x80, (byte)0x21, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosTwo_Cylinder_On(object sender, EventArgs e)
        {
            // Position Cylinder 2 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x40, (byte)0x21, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosThree_Cylinder_On(object sender, EventArgs e)
        {
            // Position Cylinder 3 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x20, (byte)0x21, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosFour_Cylinder_On(object sender, EventArgs e)
        {
            // Position Cylinder 4 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x10, (byte)0x21, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosOne_Cylinder_Off(object sender, EventArgs e)
        {
            // Position Cylinder 1 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x80, (byte)0x22, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosTwo_Cylinder_Off(object sender, EventArgs e)
        {
            // Position Cylinder 2 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x40, (byte)0x22, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosThree_Cylinder_Off(object sender, EventArgs e)
        {
            // Position Cylinder 3 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x20, (byte)0x22, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosFour_Cylinder_Off(object sender, EventArgs e)
        {
            // Position Cylinder 4 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x10, (byte)0x22, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_PosOne_Motor(object sender, EventArgs e)
        {
            // Position Motor 1 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x84, (byte)0x11, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_PosTwo_Motor(object sender, EventArgs e)
        {
            // Position Motor 2 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x44, (byte)0x11, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_PosThree_Motor(object sender, EventArgs e)
        {
            // Position Motor 3 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x24, (byte)0x11, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_PosFour_Motor(object sender, EventArgs e)
        {
            // Position Motor 4 On
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x14, (byte)0x11, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosOne_Motor_Off(object sender, EventArgs e)
        {
            // Position Motor 1 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x84, (byte)0x13, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosTwo_Motor_Off(object sender, EventArgs e)
        {
            // Position Motor 2 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x44, (byte)0x13, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosThree_Motor_Off(object sender, EventArgs e)
        {
            // Position Motor 1 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x24, (byte)0x13, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosFour_Motor_Off(object sender, EventArgs e)
        {
            // Position Motor 1 Off
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x14, (byte)0x13, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_1(object sender, EventArgs e)
        {
            // Sensor 1
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x10, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_2(object sender, EventArgs e)
        {
            // Sensor 2
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x20, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_3(object sender, EventArgs e)
        {
            // Sensor 3
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x30, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_4(object sender, EventArgs e)
        {
            // Sensor 4
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x40, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_5(object sender, EventArgs e)
        {
            // Sensor 5
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x50, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_6(object sender, EventArgs e)
        {
            // Sensor 6
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x60, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_7(object sender, EventArgs e)
        {
            // Sensor 7
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x70, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_8(object sender, EventArgs e)
        {
            // Sensor 8
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x80, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_9(object sender, EventArgs e)
        {
            // Sensor 9
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x90, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_10(object sender, EventArgs e)
        {
            // Sensor 10
            byte[] Msg = new byte[] { (byte)0x00, (byte)0xa0, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_11(object sender, EventArgs e)
        {
            // Sensor 11
            byte[] Msg = new byte[] { (byte)0x00, (byte)0xb0, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_12(object sender, EventArgs e)
        {
            // Sensor 12
            byte[] Msg = new byte[] { (byte)0x00, (byte)0xc0, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_13(object sender, EventArgs e)
        {
            // Sensor 13
            byte[] Msg = new byte[] { (byte)0x00, (byte)0xd0, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Sensor_14(object sender, EventArgs e)
        {
            // Sensor 14
            byte[] Msg = new byte[] { (byte)0x00, (byte)0xe0, (byte)0x01, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosFive_Motor_On(object sender, EventArgs e)
        {
            // bot motor forward
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x41, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_PosFive_Motor_Off(object sender, EventArgs e)
        {
            // bot motor stop
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x43, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_PosFive_Motor(object sender, EventArgs e)
        {
            // bot motor forward
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x41, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Motor1_Forward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x31, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Motor2_Forward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x34, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Motor1_Backward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x32, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Motor2_Backward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x35, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Motor1_Stop(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x33, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Motor2_Stop(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x36, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Up(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x37, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Down(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x38, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Sensor1(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x10, (byte)0x02, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Sensor2(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x20, (byte)0x02, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Sensor3(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x30, (byte)0x02, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Sensor4(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x40, (byte)0x02, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Sensor5(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x50, (byte)0x02, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Elev_Sensor6(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x60, (byte)0x02, (byte)0x80 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_Motor1_Forward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x31, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_Motor2_Forward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x34, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_Elevator_Up(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x37, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_Motor1_Backward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x32, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_Motor2_Backward(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x35, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_Elevator_Down(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x38, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_ELEV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button_Normal_PosFive_Motor_Stop(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x00, (byte)0x43, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);
            Console.WriteLine("publish = " + publishMsg);
        }

        private void button60_Click(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x16, (byte)0x11, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV1, Msg);
            
            Thread.Sleep(500);

            byte[] Msg2 = new byte[] { (byte)0x00, (byte)0x86, (byte)0x11, (byte)0x00 };
            ushort publishMsg2 = client.Publish(PUBLISH_MC_CONV2, Msg2);
        }

        private void button61_Click(object sender, EventArgs e)
        {
            byte[] Msg = new byte[] { (byte)0x00, (byte)0x16, (byte)0x11, (byte)0x00 };
            ushort publishMsg = client.Publish(PUBLISH_MC_CONV2, Msg);

            Thread.Sleep(500);

            byte[] Msg2 = new byte[] { (byte)0x00, (byte)0x02, (byte)0x34, (byte)0x00 };
            ushort publishMsg2 = client.Publish(PUBLISH_MC_ELEV2, Msg2);
        }
    }
}
