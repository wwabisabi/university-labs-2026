INSERT INTO employees (full_name, specialization, experience, rating, salary_base, salary_percent, status, is_deleted) VALUES
('Анна Петрова', 'hairdresser', 8, 4.8, 25000, 25, 'active', FALSE),
('Мария Иванова', 'manicurist', 5, 4.9, 20000, 30, 'active', FALSE),
('Елена Смирнова', 'cosmetologist', 10, 5.0, 35000, 20, 'active', FALSE),
('Ольга Соколова', 'makeupartist', 3, 4.5, 18000, 35, 'active', FALSE),
('Дарья Кузнецова', 'hairdresser', 2, 4.2, 20000, 20, 'active', FALSE),
('Наталья Попова', 'manicurist', 12, 4.7, 25000, 28, 'active', FALSE),
('Ирина Волкова', 'cosmetologist', 1, 4.0, 28000, 15, 'active', FALSE),
('Юлия Морозова', 'makeupartist', 7, 4.6, 22000, 30, 'inactive', TRUE),
('Светлана Орлова', 'hairdresser', 15, 5.0, 30000, 30, 'active', FALSE),
('Татьяна Лебедева', 'manicurist', 4, 4.4, 18000, 25, 'active', FALSE);

INSERT INTO employee_types (employee_id, type, max_clients_per_day, avg_service_time) VALUES
(1, 'hairdresser', 8, 45),
(2, 'manicurist', 6, 60),
(3, 'cosmetologist', 5, 90),
(4, 'makeupartist', 4, 75),
(5, 'hairdresser', 7, 50),
(6, 'manicurist', 7, 55),
(7, 'cosmetologist', 4, 100),
(9, 'hairdresser', 9, 40),
(10, 'manicurist', 6, 60);

INSERT INTO clients (full_name, phone, first_visit_date, total_spent, visits_count, bonus_points, is_deleted) VALUES
('Ксения Белова', '7-916-123-45-67', '2024-01-15', 12500, 8, 625, FALSE),
('Алина Фёдорова', '7-903-987-65-43', '2024-02-10', 3500, 3, 175, FALSE),
('Виктория Зайцева', '7-915-456-78-90', '2024-03-20', 7800, 5, 390, FALSE),
('Полина Романова', '7-909-234-56-78', '2024-04-05', 25000, 15, 1250, FALSE),
('София Павлова', '7-926-789-11-23', '2024-05-12', 450, 1, 22, FALSE),
('Вероника Крылова', '7-912-345-67-89', '2024-06-18', 3200, 4, 160, FALSE),
('Екатерина Николаева', '7-904-567-89-11', '2024-07-22', 18500, 11, 925, FALSE),
('Анастасия Семёнова', '7-911-890-12-34', '2024-08-30', 6800, 6, 340, FALSE),
('Марина Васильева', '7-908-123-45-67', '2024-09-08', 4200, 3, 210, FALSE),
('Дарина Алексеева', '7-913-567-89-11', '2024-10-15', 9200, 7, 460, FALSE);

INSERT INTO services (name, duration_minutes, price, required_specialization, material_cost) VALUES
('Женская стрижка', 60, 1500, 'hairdresser', 50),
('Укладка', 30, 800, 'hairdresser', 30),
('Окрашивание волос', 120, 3500, 'hairdresser', 500),
('Подравнивание кончиков', 45, 1000, 'hairdresser', 50),
('Маникюр классический', 60, 1200, 'manicurist', 100),
('Маникюр с покрытием', 50, 1300, 'manicurist', 120),
('Педикюр', 90, 2000, 'manicurist', 150),
('Снятие покрытия', 40, 800, 'manicurist', 200),
('Чистка лица', 45, 1800, 'cosmetologist', 150),
('Уходовая процедура', 60, 2200, 'cosmetologist', 200),
('Консультация косметолога', 40, 1500, 'cosmetologist', 80),
('Дневной макияж', 45, 1500, 'makeupartist', 200),
('Вечерний макияж', 60, 2500, 'makeupartist', 300),
('Свадебный макияж', 90, 4500, 'makeupartist', 500);

INSERT INTO appointments (client_id, employee_id, service_id, appointment_date, appointment_time, status, price_at_booking) VALUES
(1, 1, 1, '2024-03-15', '10:00:00', 'completed', 1500),
(1, 1, 3, '2024-03-22', '11:00:00', 'completed', 3500),
(2, 2, 5, '2024-04-10', '12:00:00', 'completed', 1200),
(3, 3, 9, '2024-05-05', '14:00:00', 'completed', 1800),
(4, 4, 13, '2024-06-18', '15:00:00', 'completed', 2500),
(4, 1, 1, '2024-06-25', '10:30:00', 'completed', 1500),
(5, 2, 6, '2024-07-12', '11:00:00', 'completed', 1300),
(6, 3, 11, '2024-08-20', '13:00:00', 'completed', 1500),
(7, 1, 4, '2024-09-05', '16:00:00', 'completed', 1000),
(7, 2, 5, '2024-09-12', '11:00:00', 'completed', 1200),
(8, 4, 12, '2024-10-28', '14:30:00', 'completed', 1500),
(9, 1, 2, '2024-11-15', '09:30:00', 'completed', 800),
(10, 3, 10, '2024-12-01', '15:00:00', 'completed', 2200),
(1, 1, 1, '2025-01-10', '11:00:00', 'scheduled', 1600),
(2, 2, 5, '2025-01-12', '12:00:00', 'scheduled', 1200),
(4, 3, 9, '2025-01-15', '14:00:00', 'scheduled', 1900);

INSERT INTO salon_state (current_salon_date, cash_balance, last_month_salary_paid, total_revenue, total_expenses) VALUES
('2025-01-10', 58750, '2024-12-01', 245300, 186550);

INSERT INTO salary_log (employee_id, month_year, total_services_sum, salary_calculated, paid_status) VALUES
(1, '2024-12-01', 45000, 36250, TRUE),
(2, '2024-12-01', 38000, 31400, TRUE),
(3, '2024-12-01', 52000, 45400, TRUE),
(4, '2024-12-01', 15000, 23250, TRUE),
(5, '2024-12-01', 25000, 25000, TRUE),
(6, '2024-12-01', 30000, 33400, TRUE),
(7, '2024-12-01', 12000, 29800, TRUE),
(9, '2024-12-01', 60000, 48000, TRUE),
(10, '2024-12-01', 20000, 23000, TRUE);