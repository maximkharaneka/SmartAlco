using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SmartAlcoEmu
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        // Add this variable

        string RxString;

        private void DisplayText(object sender, EventArgs e)
        {
            textBoxCom.AppendText(RxString);
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            serialPort.PortName = "COM2";
            serialPort.BaudRate = 9600;

            serialPort.Open();
            if (serialPort.IsOpen)
            {
                connectButton.Enabled = false;
                disconnectButton.Enabled = true;
                textBoxCom.ReadOnly = false;
            }
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                connectButton.Enabled = true;
                disconnectButton.Enabled = false;
                textBoxCom.ReadOnly = true;
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort.IsOpen) serialPort.Close();
        }

        private void textBoxCom_KeyPress(object sender, KeyPressEventArgs e)
        {
            // If the port is closed, don't try to send a character.

            if (!serialPort.IsOpen) return;

            // If the port is Open, declare a char[] array with one element.
            char[] buff = new char[1];

            // Load element 0 with the key character.

            buff[0] = e.KeyChar;

            // Send the one character buffer.
            serialPort.Write(buff, 0, 1);

            // Set the KeyPress event as handled so the character won't
            // display locally. If you want it to display, omit the next line.
            e.Handled = true;
        }

        private void serialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            RxString = serialPort.ReadExisting();
            this.Invoke(new EventHandler(DisplayText));
        }
    }
}
