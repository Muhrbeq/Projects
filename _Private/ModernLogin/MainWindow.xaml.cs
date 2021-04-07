using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Mail;

namespace ModernLogin
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if(e.LeftButton == MouseButtonState.Pressed)
            {
                this.Opacity = 0;
                DragMove();
            }
            if(e.LeftButton == MouseButtonState.Released)
            {
                this.Opacity = 1.0;
            }
        }

        private void Button_Register_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                MailMessage mail = new MailMessage();
                
                SmtpClient SmtpServer = new SmtpClient("smtp.gmail.com");
                
                mail.From = new MailAddress("rmuhrbeck.test@gmail.com");
                mail.To.Add("rasmus.muhrbeck@gmail.com");
                mail.Subject = "Test Mail";
                mail.Body = "This is for testing SMTP mail from GMAIL, usrnm:" + TextBox_Name.Text + "psw: " + PasswordBox.Password;

                SmtpServer.Port = 587;
                SmtpServer.Credentials = new System.Net.NetworkCredential("rmuhrbeck.test@gmail.com", "Stefan0512");
                SmtpServer.EnableSsl = true;

                SmtpServer.Send(mail);
                MessageBox.Show("mail Send");
                ((IDisposable)mail).Dispose();
                ((IDisposable)SmtpServer).Dispose();
            }
            catch (Exception ex)
            {
                
                MessageBox.Show(ex.ToString());
            }
        }

        private void Button_Login_Click(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Exit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
