-- Выполнять в Query Tool базы space_trainer_monitor.
-- Только чтение: данные не изменяются.
SELECT COUNT(*) AS total_measurements FROM measurements;
SELECT id, parameter_id, value, status_code, timestamp
FROM measurements ORDER BY id DESC LIMIT 20;
SELECT COUNT(*) AS invalid_status_count
FROM measurements WHERE status_code NOT IN (0,1);
SELECT p.component_id, COUNT(*) AS measurement_count
FROM measurements m JOIN parameters p ON p.id=m.parameter_id
GROUP BY p.component_id ORDER BY p.component_id;
