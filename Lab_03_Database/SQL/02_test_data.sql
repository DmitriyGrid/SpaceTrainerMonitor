
-- Лабораторная работа № 3
-- Тестовые данные SpaceTrainerMonitor
-- Скрипт рассчитан на однократное выполнение в пустых таблицах

BEGIN;

-- 1. Добавление компонентов

INSERT INTO components (name, description, current_status)
VALUES
('Контроллер управления',
 'Основной вычислительный модуль', 'NORMAL'),

('Система охлаждения',
 'Обеспечивает охлаждение оборудования', 'NORMAL'),

('Блок питания',
 'Обеспечивает электропитание компонентов', 'WARNING');


-- 2. Добавление контролируемых параметров

INSERT INTO parameters
(component_id, name, unit, min_value, max_value)
SELECT
    c.id,
    v.parameter_name,
    v.unit,
    v.min_value,
    v.max_value
FROM (
    VALUES
    ('Контроллер управления', 'Температура', 'C', 0.0, 80.0),
    ('Контроллер управления', 'Загрузка процессора', '%', 0.0, 100.0),
    ('Система охлаждения', 'Температура', 'C', 0.0, 65.0),
    ('Блок питания', 'Напряжение', 'V', 22.0, 26.0)
) AS v(component_name, parameter_name, unit, min_value, max_value)
JOIN components c ON c.name = v.component_name;


-- 3. Добавление измерений

INSERT INTO measurements (parameter_id, value, status_code)
SELECT
    p.id,
    v.measured_value,
    v.status_code
FROM (
    VALUES
    ('Контроллер управления', 'Температура', 45.5, 0),
    ('Контроллер управления', 'Температура', 48.2, 0),
    ('Контроллер управления', 'Загрузка процессора', 65.0, 0),
    ('Система охлаждения', 'Температура', 39.5, 0),
    ('Блок питания', 'Напряжение', 24.1, 0),
    ('Блок питания', 'Напряжение', 26.8, 1)
) AS v(component_name, parameter_name, measured_value, status_code)
JOIN components c ON c.name = v.component_name
JOIN parameters p ON p.component_id = c.id
                 AND p.name = v.parameter_name;


-- 4. Добавление диагностического правила

INSERT INTO diagnostic_rules
(parameter_id, rule_name, min_threshold, max_threshold, severity)
SELECT
    p.id,
    'Контроль напряжения питания',
    22.0,
    26.0,
    'WARNING'
FROM parameters p
JOIN components c ON c.id = p.component_id
WHERE c.name = 'Блок питания'
  AND p.name = 'Напряжение';


-- 5. Регистрация диагностического события

INSERT INTO diagnostic_events
(component_id, event_type, description, severity)
SELECT
    id,
    'VOLTAGE_WARNING',
    'Превышено допустимое напряжение питания',
    'WARNING'
FROM components
WHERE name = 'Блок питания';

COMMIT;
