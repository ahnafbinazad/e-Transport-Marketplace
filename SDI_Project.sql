-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Feb 12, 2022 at 11:04 AM
-- Server version: 5.7.35-0ubuntu0.18.04.2
-- PHP Version: 8.0.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `SDI_Project`
--

-- --------------------------------------------------------

--
-- Table structure for table `AccountInformation`
--

CREATE TABLE `AccountInformation` (
  `accountid` int(11) NOT NULL,
  `fullname` varchar(255) NOT NULL,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `mobilenumber` varchar(255) NOT NULL,
  `address` varchar(255) NOT NULL,
  `ninumber` varchar(255) NOT NULL,
  `drivinglicenseid` varchar(255) NOT NULL,
  `accounttype` varchar(255) NOT NULL,
  `photograph` blob NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `AccountInformation`
--

INSERT INTO `AccountInformation` (`accountid`, `fullname`, `username`, `password`, `email`, `mobilenumber`, `address`, `ninumber`, `drivinglicenseid`, `accounttype`, `photograph`) VALUES
(1, 'Berke Kanlikilic', 'berkecustomer', '123', 'berke@gmail.com', '213812845', 'address', '', '', 'customer', ''),
(2, 'Berke Kanlikilic', 'berkedriver', '123', 'berke@gmail.com', '123135', 'address', '9123591294', '1291259123', 'driver', ''),
(3, 'Berke Kanlikilic', 'berkecompany', '123', 'berke@gmail.com', '12895812', 'address', '', '', 'company', '');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `AccountInformation`
--
ALTER TABLE `AccountInformation`
  ADD PRIMARY KEY (`accountid`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `AccountInformation`
--
ALTER TABLE `AccountInformation`
  MODIFY `accountid` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
