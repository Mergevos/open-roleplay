-- phpMyAdmin SQL Dump
-- version 4.9.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Jan 02, 2021 at 11:02 PM
-- Server version: 10.4.10-MariaDB
-- PHP Version: 7.3.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `open-roleplay-database`
--

-- --------------------------------------------------------

--
-- Table structure for table `accounts`
--

DROP TABLE IF EXISTS `accounts`;
CREATE TABLE IF NOT EXISTS `accounts` (
  `account_id` int(11) NOT NULL AUTO_INCREMENT,
  `account_name` varchar(24) DEFAULT NULL,
  `account_email` varchar(40) DEFAULT NULL,
  `account_password` varchar(61) DEFAULT NULL,
  PRIMARY KEY (`account_id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `accounts`
--

INSERT INTO `accounts` (`account_id`, `account_name`, `account_email`, `account_password`) VALUES
(1, 'Sato_Ishikawa', 'sample', 'sample'),
(3, 'Sato_Ishikawas', 'moko@gm.com', 'krugukrugu'),
(4, 'Sato_Ishikawase', 'sssss@gm.cmm', '$2a$06$vTX9nJH28J3WIT3H0hDwOOGrUTrO.jzf8wKbxjoamk8wg1ejOudVS'),
(5, 'Sato_Ishikawasek', 'kkkkkkkk', '$2y$06$chOzXCvwXlXhXCvjKlTBbeyO4vvN8iptS8qnvWxpw0S5NgjKY7MJe'),
(6, 'Sato_Ishikawasekk', 'test@gm', '$2y$06$Ki74cDXVbDHWZTXwTCGxZ.rKqEYXlX1fUolWZS3EvxqyuTkX1g8kS'),
(7, 'Ato_Gay', 'krugukrugu', '$2y$06$TjH4UCrCLkrtbyTuLSiuPe5druEyLufxZG.XONWOx2RSnfgkxNTvS'),
(8, 'Mergevos', 'kita@gmail.com', '$2y$06$Zk3kP0bFRBfOOx.1QFfpaexmzmcVkrUvNw/VQS0FgVhW0CTfMArFO');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
