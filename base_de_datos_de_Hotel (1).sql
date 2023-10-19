CREATE DATABASE hotel;

\c hotel

-- Crear la tabla Catalogo Servicio
CREATE TABLE Catalogo_Servicio (
    id_cat_ser SERIAL PRIMARY KEY,
    nombre_servicio VARCHAR(50),
    precio DECIMAL(10, 2)
);

-- Crear la tabla Habitacion
CREATE TABLE Habitacion (
    id_habitacion INTEGER PRIMARY KEY,
    nombre_habitacion VARCHAR(50),
    habitaciones_disponibles INTEGER,
    precio DECIMAL(10,2)
);
INSERT INTO Habitacion VALUES(1,'Lujo',10,2499.99);
INSERT INTO Habitacion VALUES(2,'Confort',15,1699.99);
INSERT INTO Habitacion VALUES(3,'Basico',20,999.99);
-- Crear la tabla Lista_Habitacion
CREATE TABLE Lista_Habitaciones(
    numero_habitacion INTEGER,
    id_habitacion INTEGER REFERENCES Habitacion,
    estado VARCHAR(1),
    PRIMARY KEY(id_habitacion,numero_habitacion)
);
-- Insert masivo en la tabla Lista_Habitaciones
INSERT INTO Lista_Habitaciones (numero_habitacion, id_habitacion, estado)
VALUES
    (1, 1, '+'),
    (2, 1, '+'),
    (3, 1, '+'),
    (4, 1, '+'),
    (5, 1, '+'),
    (6, 1, '+'),
    (7, 1, '+'),
    (8, 1, '+'),
    (9, 1, '+'),
    (10, 1, '+'),
    (11, 2, '+'),
    (12, 2, '+'),
    (13, 2, '+'),
    (14, 2, '+'),
    (15, 2, '+'),
    (16, 2, '+'),
    (17, 2, '+'),
    (18, 2, '+'),
    (19, 2, '+'),
    (20, 2, '+'),
    (21, 2, '+'),
    (22, 2, '+'),
    (23, 2, '+'),
    (24, 2, '+'),
    (25, 2, '+'),
    (26, 3, '+'),
    (27, 3, '+'),
    (28, 3, '+'),
    (29, 3, '+'),
    (30, 3, '+'),
    (31, 3, '+'),
    (32, 3, '+'),
    (33, 3, '+'),
    (34, 3, '+'),
    (35, 3, '+'),
    (36, 3, '+'),
    (37, 3, '+'),
    (38, 3, '+'),
    (39, 3, '+'),
    (40, 3, '+'),
    (41, 3, '+'),
    (42, 3, '+'),
    (43, 3, '+'),
    (44, 3, '+'),
    (45, 3, '+');


-- Crear la tabla Cliente
CREATE TABLE Cliente (
    id_cliente SERIAL PRIMARY KEY,
    nombre VARCHAR(50),
    ap_pat VARCHAR(50),
    ap_mat VARCHAR(50),
    rfc VARCHAR(13) UNIQUE,
    direccion TEXT,
    telefono VARCHAR(15),
    CHECK (telefono IN ('casa', 'personal', 'oficina')) -- Asegura que el teléfono sea uno de estos valores
);


-- Crear la tabla Reservacion
CREATE TABLE Reservacion (
    id_res SERIAL PRIMARY KEY,
    fecha DATE,
    rfc VARCHAR(13),
    hora TIME,
    id_habitacion INTEGER, -- ID de la habitación según la tabla "Habitacion"
    numero_habitacion INTEGER, -- Número de habitación según la tabla "Lista_Habitaciones"
    fecha_llegada DATE,
    fecha_salida DATE,
    FOREIGN KEY (rfc) REFERENCES Cliente(rfc),
    FOREIGN KEY (id_habitacion) REFERENCES Habitacion(id_habitacion),
    FOREIGN KEY (id_habitacion,numero_habitacion) REFERENCES Lista_Habitaciones(id_habitacion,numero_habitacion)
);

/*CREATE TABLE Reservacion (
    id_res SERIAL PRIMARY KEY,
    fecha DATE,
    rfc VARCHAR(13),
    hora TIME,
    tipo_habitacion VARCHAR(50),
    fecha_llegada DATE,
    fecha_salida DATE,
    FOREIGN KEY (rfc) REFERENCES Cliente(rfc)
);*/

-- Crear la tabla Estancia
CREATE TABLE Estancia (
    id_esta SERIAL PRIMARY KEY,
    nombre_cliente VARCHAR(50),
    numero_habitacion VARCHAR(50),
    fecha DATE,
    hora TIME,
    dias_estancia INT,
    servicios_consumidos TEXT
);

-- Crear la tabla Cancelacion
CREATE TABLE Cancelacion (
    id_can SERIAL PRIMARY KEY,
    fecha DATE,
    hora TIME,
    id_reservacion INT,
    rfc VARCHAR(16),
    FOREIGN KEY (id_reservacion) REFERENCES Reservacion(id_res),
    FOREIGN KEY (rfc) REFERENCES Cliente(rfc)
);

-- Crear la tabla Facturacion
CREATE TABLE Facturacion (
    id_fact SERIAL PRIMARY KEY,
    fecha DATE,
    hora TIME,
    rfc VARCHAR(13),
    monto DECIMAL(10, 2),
    descripcion_servicio TEXT,
    id_estancia INT,
    id_cancelacion INT,
    FOREIGN KEY (id_estancia) REFERENCES Estancia(id_esta),
    FOREIGN KEY (id_cancelacion) REFERENCES Cancelacion(id_can)
);

-- Crear la tabla Servicio
CREATE TABLE Servicio (
    id_ser SERIAL PRIMARY KEY,
    nombre_servicio VARCHAR(50),
    precio DECIMAL(10, 2),
    id_estancia INT,
    id_cat_ser INT REFERENCES Catalogo_Servicio(id_cat_ser)
);

CREATE TABLE Telefonos (
    id_telefono SERIAL PRIMARY KEY,
    id_cliente INTEGER REFERENCES Cliente(id_cliente),
    tipo_telefono VARCHAR(15) CHECK (tipo_telefono IN ('casa', 'personal', 'oficina')),
    numero_telefono VARCHAR(15),
    UNIQUE (id_cliente, tipo_telefono) -- Asegura que solo haya un número por tipo para cada cliente
);
INSERT INTO Telefonos (id_cliente, tipo_telefono, numero_telefono)
VALUES (1, 'personal', '9622717259');



-- Eliminar la tabla Servicio
DROP TABLE Servicio;

-- Eliminar la tabla Facturacion
DROP TABLE Facturacion;

-- Eliminar la tabla Cancelacion
DROP TABLE Cancelacion;

-- Eliminar la tabla Estancia
DROP TABLE Estancia;

-- Eliminar la tabla Reservacion
DROP TABLE Reservacion;

-- Eliminar la tabla Cliente
DROP TABLE Cliente;

-- Eliminar la tabla Habitacion
DROP TABLE Habitacion;

-- Eliminar la tabla Catalogo Servicio
DROP TABLE Catalogo_Servicio;

--Eliminar la tabla Telefonos
DROP TABLE Telefonos;