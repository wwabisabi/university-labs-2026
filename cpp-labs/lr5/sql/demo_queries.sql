SELECT * FROM employees ORDER BY id;

SELECT * FROM clients ORDER BY id;

SELECT * FROM services ORDER BY id;

SELECT * FROM appointments ORDER BY id;

SELECT * FROM salon_state;

SELECT * FROM salary_log ORDER BY id;

SELECT
    a.id,
    c.full_name AS client_name,
    e.full_name AS employee_name,
    s.name AS service_name,
    a.appointment_date,
    a.appointment_time,
    a.status,
    a.price_at_booking
FROM appointments a
JOIN clients c ON c.id = a.client_id
JOIN employees e ON e.id = a.employee_id
JOIN services s ON s.id = a.service_id
ORDER BY a.id;

SELECT
    e.id,
    e.full_name,
    e.specialization,
    e.salary_base,
    e.salary_percent
FROM employees e
WHERE e.is_deleted = FALSE
ORDER BY e.id;

SELECT
    c.id,
    c.full_name,
    c.total_spent,
    c.visits_count,
    c.bonus_points
FROM clients c
WHERE c.is_deleted = FALSE
ORDER BY c.total_spent DESC;