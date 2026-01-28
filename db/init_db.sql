-- ----------------------------
-- Создание пользователя для приложения
-- ----------------------------
DO
$$
BEGIN
   IF NOT EXISTS (
       SELECT FROM pg_catalog.pg_roles WHERE rolname = 'calc_user'
   ) THEN
       CREATE USER calc_user WITH PASSWORD 'calc_pass';
   END IF;
END
$$;

-- ----------------------------
-- Создание базы
-- ----------------------------
-- DROP DATABASE IF EXISTS calc;
CREATE DATABASE calc OWNER calc_user;

-- ----------------------------
-- Подключаемся к базе (под postgres)
-- ----------------------------
\c calc

-- ----------------------------
-- Таблица для кэша вычислений
-- ----------------------------
CREATE TABLE IF NOT EXISTS calc_cache (
    lhs BIGINT NOT NULL,            
    rhs BIGINT NOT NULL,            
    op CHAR(1) NOT NULL,            
    result BIGINT NOT NULL,         
    created_at TIMESTAMP DEFAULT now(), 
    PRIMARY KEY (lhs, rhs, op)     
);

-- Индекс по операции
CREATE INDEX IF NOT EXISTS idx_calc_cache_op ON calc_cache(op);

-- ----------------------------
-- Даем права пользователю calc_user
-- ----------------------------
ALTER TABLE calc_cache OWNER TO calc_user;
ALTER INDEX idx_calc_cache_op OWNER TO calc_user;
GRANT ALL PRIVILEGES ON TABLE calc_cache TO calc_user;

-- ----------------------------
-- Очищаем таблицу перед наполнением
-- ----------------------------
TRUNCATE TABLE calc_cache;

-- ----------------------------
-- Вставка ручных примеров
-- ----------------------------
INSERT INTO calc_cache(lhs, rhs, op, result) VALUES
    (2, 3, '+', 5),
    (10, 5, '-', 5),
    (4, 7, 'x', 28),   
    (20, 4, '/', 5),
    (0, 0, '+', 0),
    (100, 25, '-', 75),
    (6, 6, 'x', 36),   
    (15, 3, '/', 5)
ON CONFLICT(lhs, rhs, op) DO NOTHING;

-- ----------------------------
-- Генерация комбинаций 1..10 для тестирования
-- ----------------------------
INSERT INTO calc_cache(lhs, rhs, op, result)
SELECT i, j, op, 
       CASE op
           WHEN '+' THEN i + j
           WHEN '-' THEN i - j
           WHEN 'x' THEN i * j
           WHEN '/' THEN CASE WHEN j != 0 THEN i / j ELSE 0 END
       END
FROM generate_series(1, 10) AS i,
     generate_series(1, 10) AS j,
     (VALUES ('+'), ('-'), ('x'), ('/')) AS ops(op)
ON CONFLICT(lhs, rhs, op) DO NOTHING;

-- ----------------------------
-- Готово
-- ----------------------------
\echo "Database calc, user calc_user created and cache table filled."
