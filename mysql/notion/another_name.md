在 MySQL 里，别名是一种为表或列赋予临时名称的方式，主要用于提升查询的可读性与简洁性，特别是在涉及多表连接、子查询或者聚合函数的复杂查询中，别名的作用尤为明显。下面为你详细介绍表别名和列别名的使用方法。

### 列别名
#### 语法
在 `SELECT` 语句里，通过 `AS` 关键字为列指定别名，`AS` 关键字可省略。基本语法如下：
```sql
SELECT column_name AS alias_name
FROM table_name;
```
#### 示例
假设存在一个 `products` 表，其中包含 `product_name` 和 `price` 列，若要查询产品名称和价格，并为 `price` 列指定别名 `product_price`，可以使用如下 SQL 语句：
```sql
SELECT product_name, price AS product_price
FROM products;
```
若省略 `AS` 关键字，语句同样有效：
```sql
SELECT product_name, price product_price
FROM products;
```
#### 用途
- **提高可读性**：当列名较长或者含义不够清晰时，使用别名能让查询结果更易理解。
- **聚合函数结果命名**：在使用聚合函数（如 `SUM()`、`AVG()` 等）时，使用别名可以为计算结果赋予有意义的名称。例如：
```sql
SELECT SUM(quantity) AS total_quantity
FROM order_items;
```

### 表别名
#### 语法
在 `FROM` 子句或者 `JOIN` 子句中，通过 `AS` 关键字为表指定别名，`AS` 关键字同样可省略。基本语法如下：
```sql
SELECT column1, column2
FROM table_name1 AS alias1
JOIN table_name2 AS alias2
ON alias1.column = alias2.column;
```
#### 示例
假设存在 `employees` 表和 `departments` 表，要查询员工姓名和所在部门名称，可以使用表别名来简化查询语句：
```sql
SELECT e.name, d.department_name
FROM employees e
JOIN departments d
ON e.department_id = d.department_id;
```
#### 用途
- **简化查询语句**：当表名较长或者在多表连接中多次引用同一个表时，使用表别名能让查询语句更加简洁。
- **区分同名列**：在多表连接中，不同表可能存在同名的列，使用表别名可以明确指定要查询的列来自哪个表。例如：
```sql
SELECT e.employee_id, d.department_id
FROM employees e
JOIN departments d
ON e.department_id = d.department_id;
```

### 注意事项
- **别名的作用域**：别名仅在当前查询中有效，不会影响表或列的实际名称。
- **空格问题**：在使用列别名时，如果别名包含空格，需要使用引号将别名括起来。例如：
```sql
SELECT price AS "Product Price"
FROM products;
```

通过合理使用别名，可以让 SQL 查询语句更加清晰易懂，提高代码的可维护性。 