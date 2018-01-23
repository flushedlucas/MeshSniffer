-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Tempo de geração: 22/01/2018 às 13:41
-- Versão do servidor: 10.1.25-MariaDB
-- Versão do PHP: 7.1.7

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
-- Estrutura para tabela `dadosCapturados`
--

CREATE TABLE `dadosCapturados` (
  `id` int(8) NOT NULL,
  `nodeId` varchar(45) NOT NULL,
  `rssi` varchar(5) NOT NULL,
  `ch` varchar(5) NOT NULL,
  `addr` varchar(20) NOT NULL,
  `ssid` varchar(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Índices de tabelas apagadas
--

--
-- Índices de tabela `dadosCapturados`
--
ALTER TABLE `dadosCapturados`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT de tabelas apagadas
--

--
-- AUTO_INCREMENT de tabela `dadosCapturados`
--
ALTER TABLE `dadosCapturados`
  MODIFY `id` int(8) NOT NULL AUTO_INCREMENT;COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
