--CREATE DATABASE aseguradora;
drop table if exists t_propietario CASCADE;
CREATE TABLE t_propietario(
  id_propietario serial not null,
  nombre varchar(30) not null,
  ape_paterno varchar(30) not null,
  ape_materno varchar(30) not null,
  edad int not null,
  sexo varchar(10) not null,
  direccion varchar(50) not null,
  rfc varchar(13) not null,
  CONSTRAINT rfc_unique UNIQUE (rfc),
  PRIMARY KEY(id_propietario)
);
drop table if exists t_auto CASCADE;
CREATE TABLE t_auto(
  placa varchar(15) not null,
  --CHIS-93D-233
  id_propietario1 int not null,
  color varchar(15) not null,
  --verde y rojo
  marca varchar(20) not null,
  modelo int not null,
  PRIMARY KEY(placa),
  CONSTRAINT fk_idPropietario FOREIGN KEY(id_propietario1) REFERENCES t_propietario(id_propietario) ON UPDATE NO ACTION ON DELETE CASCADE
);
drop table if exists t_accidente;
CREATE TABLE t_accidente(
  id_accidente serial not null,
  placa1 varchar(15) not null,
  fecha date not null,
  lugar varchar(50) not null,
  hora_formato24 int not null,
  PRIMARY KEY(id_accidente),
  CONSTRAINT fk_placaAuto FOREIGN KEY(placa1) REFERENCES t_auto(placa) ON UPDATE NO ACTION ON DELETE CASCADE
);
drop table if exists t_telefono;
alter table t_telefono
add CONSTRAINT pk_idpro PRIMARY KEY id_propietario1 CREATE TABLE t_telefono(
    id_tel serial not null,
    id_propietario1 int not null,
    tel_casa varchar(12) not null default 'Ninguno',
    tel_celular varchar(12) not null default 'Ninguno',
    tel_oficina varchar(12) not null default 'Ninguno',
    PRIMARY KEY(id_tel),
    CONSTRAINT fk_idPropietario FOREIGN KEY(id_propietario1) REFERENCES t_propietario(id_propietario) ON UPDATE NO ACTION ON DELETE CASCADE
  );
------------------- CONSTARINT 
--modelo de auto > 2015
ALTER TABLE t_auto
ADD CONSTRAINT modelo_Mayor2015 CHECK (modelo >= 2015);
--edad > 18 años y hora_accidente no mayor a 24 ya que se utilizara formto 24 horas
alter TABLE t_accidente
add CONSTRAINT formato_24hrs CHECK (hora_accidente <= 24);
alter TABLE t_propietario
add CONSTRAINT mayor18 check (edad >= 18);
/ / Remember que no debe de tener una vista el id para poder modificarla
ALTER TABLE t_propietario
ALTER COLUMN id_propietario TYPE int;
-----------------eliminar y consultar tblas
select *
from t_telefono
WHERE id_propietario1 = 9;
;
select *
from t_accidente;
select *
from t_propietario
select *
from t_auto;
DELETE from t_telefono
WHERE id_tel = 9 --drop
  drop table t_telefono;
drop table t_accidente;
drop table t_propietario CASCADE;
drop table t_auto;
DROP VIEW vista_propietarios;
drop view vista_autos DROP view vista_propietariosTelefono ----------------Functions and Procedure Para respaldos de cada modificacion se ha hecho en la basededatos
--insert s a cada tablaa
insert into t_propietario (
    id_propietario,
    nombre,
    ape_paterno,
    ape_materno,
    edad,
    sexo,
    direccion,
    rfc
  )
values(
    6,
    'Citlalys',
    'Cejas',
    'Rojas',
    19,
    'femenino',
    'Escuintla',
    'Affdfdf'
  );
insert into t_propietario (
    id_propietario,
    nombre,
    ape_paterno,
    ape_materno,
    edad,
    sexo,
    direccion,
    rfc
  )
values(
    6,
    'Citlalys',
    'Cejas',
    'Rojas',
    19,
    'femenino',
    'Escuintla',
    'ASDA8710M31WO'
  );
insert into t_propietario (
    nombre,
    ape_paterno,
    ape_materno,
    edad,
    sexo,
    direccion,
    rfc
  )
values(
    'mama de Citlalys',
    'Cejas',
    'flores',
    45,
    'femenino',
    'Escuintla',
    'MACT789812K2'
  );
insert into t_telefono (id_propietario1, tel_casa, tel_celular, tel_oficina)
values (1, default, '9612344532', default);
insert into t_auto (placa, id_propietario1, color, marca, modelo)
values ('CHIS-TAP-T01', 1, 'negro', 'NISSAN', 2016);
insert into t_accidente (placa1, fecha, lugar, hora_formato24)
values ('CHIS-TAP01', '22/12/10', '4norte y central tap', 8);
insert into t_telefono (id_propietario1, tel_casa, tel_celular, tel_oficina)
values (2, '9621009890', default, '9515550125');
truncate t_accidente;
--------------- Creamos vistas para cuando el usuario vaya a registra datos a las tablas muestre unicamene los datos que se soliciten conocer
---------------- vista de propietarios para regiestrar un auto
CREATE VIEW vista_propietarios AS
select id_propietario,
  nombre,
  ape_paterno,
  ape_materno
from t_propietario;
select *
from vista_propietarios;
--vista de autos para registrar un accidente
CREATE OR REPLACE VIEW vista_autos (PlacasRegistradas) AS
select placa,
  marca,
  modelo
from t_auto;
select *
from vista_autos;
--WHERE
--  p.id_propietario = 2;
select *
from vista_propietariosTelefono
WHERE id_propietario = 4


CREATE or replace VIEW vistaleft_propietariosTelefono AS
SELECT DISTINCT (p.id_propietario),
  p.nombre,
  p.ape_paterno,
  p.ape_materno,
  p.edad,
  p.sexo,
  p.direccion,
  p.rfc,
  t.tel_casa,
  t.tel_celular,
  t.tel_oficina
FROM t_propietario p
  LEFT JOIN t_telefono t ON t.id_propietario1 = p.id_propietario --WHERE
  --  p.id_propietario = 2;
  
select *
from vista_propietariosTelefono
select *
from DROP VIEW  vistaleft_propietariosTelefono
  /*------- creamos tabla para Generar los reportes de cada tbala para hacer un test*/
  CREATE TABLE log_t_propietario(
    id_registro_log serial not null PRIMARY KEY,
    name_modificador varchar(25) default current_user,
    fechaModificacion date default current_date,
    id_propietario1 int not null,
    nombre varchar(30) not null,
    ape_paterno varchar(30) not null,
    ape_materno varchar(30) not null,
    edad int not null,
    sexo varchar(10) not null,
    direccion varchar(50) not null,
    rfc varchar(13) not null
  );
select *
from log_t_propietario;
CREATE OR REPLACE FUNCTION sp_logPropietarios() RETURNS trigger AS $$ begin
INSERT INTO log_t_propietario (
    id_propietario1,
    nombre,
    ape_paterno,
    ape_materno,
    edad,
    sexo,
    direccion,
    rfc
  )
values (
    old.id_propietario,
    old.nombre,
    old.ape_paterno,
    old.ape_materno,
    old.edad,
    old.sexo,
    old.direccion,
    old.rfc
  );
RETURN new;
END $$ LANGUAGE 'plpgsql';
---//ahor apreparamos el trigger SE ACTIVE ANTES DE HACER UPDATE O ELIMINAR 
CREATE or replace trigger tr_update_propietario BEFORE
UPDATE ON t_propietario for each row execute procedure sp_logPropietarios();
/ / -------abhora bien vamos hacer la tabla autos
CREATE TABLE log_t_auto(
  id_registro_log serial not null PRIMARY KEY,
  name_modificador varchar(25) default current_user,
  fechaModificacion date default current_date,
  placa varchar(15) not null,
  --CHIS-93D-233
  id_propietario1 int not null,
  color varchar(15) not null,
  --verde y rojo
  marca varchar(20) not null,
  modelo int not null
);
CREATE OR REPLACE FUNCTION sp_logAutos() RETURNS trigger AS $$ begin
INSERT INTO log_t_auto (placa, id_propietario1, color, marca, modelo)
values (
    old.placa,
    old.id_propietario1,
    old.color,
    old.marca,
    old.modelo
  );
RETURN new;
END $$ LANGUAGE 'plpgsql';
---//ahor apreparamos el trigger SE ACTIVE ANTES DE HACER UPDATE O ELIMINAR 
CREATE or replace trigger tr_update_auto BEFORE
UPDATE ON t_auto for each row execute procedure sp_logAutos();
SELECT *
FROM log_t_auto --///-----AHORA BIEN VAMOS POR LA TABLA ACCIDENTES
  CREATE TABLE log_t_accidente(
    id_registro_log serial not null PRIMARY KEY,
    name_modificador varchar(25) default current_user,
    fechaModificacion date default current_date,
    id_accidente serial not null,
    placa1 varchar(15) not null,
    fecha date not null,
    lugar varchar(50) not null,
    hora_formato24 int not null
  );
CREATE OR REPLACE FUNCTION sp_logAccidentes() RETURNS trigger AS $$ begin
INSERT INTO log_t_accidente (
    id_accidente,
    placa1,
    fecha,
    lugar,
    hora_formato24
  )
values (
    old.id_accidente,
    old.placa1,
    old.fecha,
    old.lugar,
    old.hora_formato24
  );
RETURN new;
END $$ LANGUAGE 'plpgsql';
---//ahor apreparamos el trigger SE ACTIVE ANTES DE HACER UPDATE O ELIMINAR 
CREATE or replace trigger tr_update_accidente BEFORE
UPDATE ON t_accidente for each row execute procedure sp_logAccidentes();
SELECT *
FROM log_t_accidente
SELECT *
FROM t_accidente