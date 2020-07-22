-- comment

CREATE TABLE stuff COLUMNS(col1 INT, col2 Varchar);
SELECT * FROM stuff WHERE id = 'string';
select * from stuff where id < 0.42;
Select col1, col2 From stuff Where stuff.col1 IS NOT NuLL;

CREATE TABLE Persons (
    ID int NOT NULL AUTO_INCREMENT,
    LastName varchar(255) NOT NULL,
    FirstName varchar(255),
    Age int,
    PRIMARY KEY (ID)
    CONSTRAINT CHK_Person CHECK (Age>=18 AND City='Sandnes')
);

SELECT column_name(s)
FROM table1
FULL OUTER JOIN table2 ON table1.column_name = table2.column_name;

SELECT * FROM Customers
WHERE City LIKE '[bsp]%';

SELECT Count(*) AS DistinctCountries
FROM (SELECT DISTINCT Country FROM Customers)
GROUP BY Country
HAVING COUNT(CustomerID) > 5
ORDER BY COUNT(CustomerID) DESC;

SELECT 'Customer' As Type, ContactName, City, Country
FROM Customers
UNION
SELECT 'Supplier', ContactName, City, Country
FROM Suppliers;

SELECT COUNT(ProductID), AVG(Price)
FROM Products;

SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET default_tablespace = '';

DROP TABLE IF EXISTS employees;

ALTER TABLE ONLY employees
    ADD CONSTRAINT fk_employees_employees FOREIGN KEY (reportsto) REFERENCES employees;

CREATE VIEW [Current Product List] AS
SELECT ProductID, ProductName
FROM Products
WHERE Discontinued = No;

SELECT * FROM [Current Product List];

SELECT * FROM [Category Sales For 1997]
WHERE CategoryName = 'Beverages';

begin
end
/
 /
/ u
