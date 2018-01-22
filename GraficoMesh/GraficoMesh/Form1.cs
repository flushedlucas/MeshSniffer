using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using System.Windows.Forms.DataVisualization.Charting;

namespace GraficoMesh
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            maskedTextBox2.Text = DateTime.Now.ToShortDateString();
        }

        //combo travada
        private void comboBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            #region Quantidade de Mac's por intervalo de tempo (Grafico)
            if (comboBox1.Text.Equals("Quantidade de Mac's por intervalo de tempo (Grafico)"))
            {
                //define string de conexao e cria a conexao
                DataSet mDataSet = new DataSet();

                MySqlConnection mConn = new MySqlConnection();
                mConn.ConnectionString = "Server=localhost;Database=redeMesh;Uid=root;Pwd=;";
                try
                {
                    mConn.Open();
                    //abre a conexao                    
                }
                catch (Exception)
                {
                    throw;
                }

                //verificva se a conexão esta aberta
                if (mConn.State == ConnectionState.Open)

                {
                    CleanChart(chart1);
                    dataGridView1.Visible = false;
                    string query = "SELECT COUNT(Mac) as total, Mac  FROM dados where dataAlter >= '" + maskedTextBox1.Text.Trim() + "' and dataAlter >= '"+ maskedTextBox2.Text.Trim() + "' group by Ssid";
                    //MessageBox.Show(query);
                    MySqlCommand cdmDataBase = new MySqlCommand(query, mConn);
                    MySqlDataReader myReader;
                    

                    myReader = cdmDataBase.ExecuteReader();
                    while (myReader.Read())
                    {
                        this.chart1.Series["dados"].Points.AddXY(myReader.GetString("Mac"), myReader.GetInt32("total"));
                    }

                    chart1.Visible = true;
                   // MessageBox.Show("GRAFICAR!!!!");
                }

            }
            #endregion

            #region Limpar Tela
            if (comboBox1.Text.Equals("Escolha uma opção:"))
            {
                CleanChart(chart1);
                dataGridView1.Visible = false;
                chart1.Visible = false;
            }
            #endregion

            #region Dados Capturados (Tabela)
            if (comboBox1.Text.Equals("Dados Capturados (Tabela)"))
            {
                //define string de conexao e cria a conexao
                DataSet mDataSet = new DataSet();
                chart1.Visible = false;

                MySqlConnection mConn = new MySqlConnection();
                mConn.ConnectionString = "Server=localhost;Database=redeMesh;Uid=root;Pwd=;";
                try
                {
                    //abre a conexao
                    mConn.Open();
                }
                catch (Exception)
                {
                    throw;
                }

                //verificva se a conexão esta aberta

                if (mConn.State == ConnectionState.Open)

                {

                    //cria um adapter usando a instrução SQL para acessar a tabela Clientes

                    MySqlDataAdapter mAdapter = new MySqlDataAdapter("SELECT * FROM dados", mConn);

                    //preenche o dataset via adapter

                    mAdapter.Fill(mDataSet, "dados");

                    //atribui a resultado a propriedade DataSource do DataGrid

                    dataGridView1.DataSource = mDataSet;

                    dataGridView1.DataMember = "dados";
                    dataGridView1.Columns["idDados"].Width = 50;
                    dataGridView1.Columns["dataAlter"].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;

                    dataGridView1.Visible = true;
                }

            }
            #endregion

            #region Número de Usuario por faixa de potência
            if (comboBox1.Text.Equals("Número de Usuario por faixa de potência"))
            {
                //define string de conexao e cria a conexao
                DataSet mDataSet = new DataSet();

                MySqlConnection mConn = new MySqlConnection();
                mConn.ConnectionString = "Server=localhost;Database=redeMesh;Uid=root;Pwd=;";
                try
                {
                    mConn.Open();
                    //abre a conexao                    
                }
                catch (Exception)
                {
                    throw;
                }

                //verificva se a conexão esta aberta
                if (mConn.State == ConnectionState.Open)

                {
                    CleanChart(chart1);
                    dataGridView1.Visible = false;
                    string query = "SELECT COUNT(Rssi) as total, Rssi  FROM dados where dataAlter >= '" + maskedTextBox1.Text.Trim() + "' and dataAlter >= '" + maskedTextBox2.Text.Trim() + "' group by Ssid";
                    //MessageBox.Show(query);
                    MySqlCommand cdmDataBase = new MySqlCommand(query, mConn);
                    MySqlDataReader myReader;


                    myReader = cdmDataBase.ExecuteReader();
                    while (myReader.Read())
                    {
                        this.chart1.Series["dados"].Points.AddXY(myReader.GetString("Rssi"), myReader.GetInt32("total"));
                    }

                    //chart1.Series. = SeriesChartType.Point;
                    chart1.Visible = true;
                    // MessageBox.Show("GRAFICAR!!!!");
                }

            }
            #endregion

        }


        //Limpar dados dos graficos
        public static void CleanChart(Chart currentChart)
        {
            foreach (var itemSerie in currentChart.Series)
            {
                if (itemSerie.Points != null)
                    itemSerie.Points.Clear();
            }
        }
    }
}
