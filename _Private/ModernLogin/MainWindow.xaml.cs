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
                this.Opacity = 0.5;
                DragMove();
            }
            if(e.LeftButton == MouseButtonState.Released)
            {
                this.Opacity = 1.0;
            }
        }

        private void Button_Register_Click(object sender, RoutedEventArgs e)
        {
            SendMail();
        }

        private void Button_Login_Click(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Exit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private bool SendMail()
        {
            try
            {
                /* Check for internet connection!! otherwise just save to db */
                MailMessage mail = new MailMessage();

                SmtpClient SmtpServer = new SmtpClient("smtp.gmail.com");

                mail.From = new MailAddress("rmuhrbeck.test@gmail.com");
                mail.To.Add(TextBox_Name.Text);
                mail.Subject = "Registation mail";
                mail.Body = "Your account has been created and added to the database\n" + "Username: " + TextBox_Name.Text + "\n" + "Password: " + PasswordBox.Password;

                SmtpServer.Port = 587;
                SmtpServer.Credentials = new System.Net.NetworkCredential("rmuhrbeck.test@gmail.com", "Stefan0512");
                SmtpServer.EnableSsl = true;

                SmtpServer.Send(mail);
                MessageBox.Show("mail Send");
                ((IDisposable)mail).Dispose();
                ((IDisposable)SmtpServer).Dispose();

                return true;
            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.ToString());
            }

            return false;
        }

        private bool CheckForCredentials()
        {
            return true;
        }
    }
}
