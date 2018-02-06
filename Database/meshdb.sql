-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: 06-Fev-2018 às 09:25
-- Versão do servidor: 5.5.58-0ubuntu0.14.04.1
-- PHP Version: 7.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Banco de dados: `meshdb`
--
CREATE DATABASE IF NOT EXISTS `meshdb` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `meshdb`;

-- --------------------------------------------------------

--
-- Estrutura para tabela `dadosMesh`
--

CREATE TABLE `dadosCapturados` (
  `id` int(15) NOT NULL,
  `nodeId` varchar(20) NOT NULL,
  `rssi` varchar(5) NOT NULL,
  `ch` varchar(5) NOT NULL,
  `addr` varchar(20) NOT NULL,
  `ssid` varchar(60) NOT NULL,
  `datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Índices de tabelas apagadas
--

--
-- Índices de tabela `dadosCapturados`
--
ALTER TABLE `dadosMesh`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `dadosMesh`
--
ALTER TABLE `dadosCapturados`
  MODIFY `id` int(15) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2778;COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
