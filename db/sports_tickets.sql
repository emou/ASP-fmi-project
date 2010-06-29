SET search_path TO tickets;
-- -----------------------------------------------------
-- Table "User"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "User"  CASCADE;

CREATE TABLE "User" (
  "email" VARCHAR(100) NOT NULL ,
  "password" VARCHAR(128) NOT NULL ,
  "first_name" VARCHAR(45) NULL ,
  "last_name" VARCHAR(45) NULL ,
  "is_admin" BOOLEAN NULL DEFAULT FALSE ,
  PRIMARY KEY ("email") )
;

-- -----------------------------------------------------
-- Table "Address"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Address"  CASCADE;

CREATE TABLE "Address" (
  "id" SERIAL ,
  "city" VARCHAR(60) NOT NULL ,
  "details" VARCHAR(100) NULL ,
  PRIMARY KEY ("id") )
;

-- -----------------------------------------------------
-- Table "Client"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Client"  CASCADE;

CREATE TABLE "Client" (
  "User_email" VARCHAR(100) NOT NULL ,
  "phone_number" VARCHAR(25) NOT NULL ,
  "Address_id" INT NOT NULL ,
  PRIMARY KEY ("User_email") ,
  CONSTRAINT "fk_Client_User"
    FOREIGN KEY ("User_email")
    REFERENCES "User" ("email")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Client_Address"
    FOREIGN KEY ("Address_id" )
    REFERENCES "Address" ("id" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
;

CREATE INDEX "fk_Client_User" ON "Client" ("User_email" ASC) ;

CREATE INDEX "fk_Client_Address" ON "Client" ("Address_id" ASC) ;

-- -----------------------------------------------------
-- Table "Place"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Place"  CASCADE;

CREATE TABLE "Place" (
  "name" VARCHAR(100) NOT NULL ,
  "Address_id" INT NOT NULL ,
  "description" VARCHAR(255) NULL ,
  PRIMARY KEY ("name") ,
  CONSTRAINT "fk_Place_Address"
    FOREIGN KEY ("Address_id" )
    REFERENCES "Address" ("id" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
;

CREATE INDEX "fk_Place_Address" ON "Place" ("Address_id" ASC) ;


-- -----------------------------------------------------
-- Table "Administrator"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Administrator"  CASCADE;

CREATE TABLE "Administrator" (
  "User_email" VARCHAR(100) NOT NULL ,
  PRIMARY KEY ("User_email") ,
  CONSTRAINT "fk_Administrator_User"
    FOREIGN KEY ("User_email" )
    REFERENCES "User" ("email" )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
;

CREATE INDEX "fk_Administrator_User" ON "Administrator" ("User_email" ASC) ;


-- -----------------------------------------------------
-- Table "Sport"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Sport"  CASCADE;

CREATE TABLE "Sport" (
  "name" VARCHAR(100) NOT NULL ,
  "typical_duration" TIME NULL ,
  "event_name" VARCHAR(45) NULL ,
  PRIMARY KEY ("name") )
;


-- -----------------------------------------------------
-- Table "Event"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Event"  CASCADE;
CREATE  TABLE "Event" (
  "id" SERIAL ,
  "Place_name" VARCHAR(150) NOT NULL ,
  "Sport_name" VARCHAR(100) NOT NULL ,
  "start" TIMESTAMP NOT NULL ,
  "duration" TIME NULL ,
  "name" VARCHAR(100) NULL ,
  PRIMARY KEY ("id") ,
  CONSTRAINT "fk_Event_Sport"
    FOREIGN KEY ("Sport_name" )
    REFERENCES "Sport" ("name" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT "fk_Event_Place"
    FOREIGN KEY ("Place_name" )
    REFERENCES "Place" ("name" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
;

CREATE INDEX "fk_Event_Sport" ON "Event" ("Sport_name" ASC) ;
CREATE INDEX "fk_Event_Place" ON "Event" ("Place_name" ASC) ;


-- -----------------------------------------------------
-- Table "TicketCategory"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "TicketCategory"  CASCADE;

CREATE TABLE "TicketCategory" (
  "id" SERIAL ,
  "name" VARCHAR(45) NOT NULL ,
  "price" DECIMAL(9,2) NOT NULL ,
  "Event_id" INT NOT NULL ,
  "count" INT NULL ,
  PRIMARY KEY ("id") ,
  CONSTRAINT "fk_Ticket_Event"
    FOREIGN KEY ("Event_id" )
    REFERENCES "Event" ("id" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
;

CREATE INDEX "fk_Ticket_Event" ON "TicketCategory" ("Event_id" ASC) ;


-- -----------------------------------------------------
-- Table "Courier"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Courier"  CASCADE;

CREATE TABLE "Courier" (
  "name" VARCHAR(100) NOT NULL ,
  "calculate_url" VARCHAR(120) NOT NULL ,
  PRIMARY KEY ("name") )
;


-- -----------------------------------------------------
-- Table "Order"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Order"  CASCADE;

CREATE TABLE "Order" (
  "id" SERIAL ,
  "User_email" VARCHAR(100) NOT NULL ,
  "created_at" TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  "Courier_name" VARCHAR(100) NOT NULL ,
  PRIMARY KEY ("id") ,
  CONSTRAINT "fk_Order_User"
    FOREIGN KEY ("User_email" )
    REFERENCES "User" ("email" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT "fk_Order_Courier"
    FOREIGN KEY ("Courier_name" )
    REFERENCES "Courier" ("name" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
;

CREATE INDEX "fk_Order_User" ON "Order" ("User_email" ASC) ;

CREATE INDEX "fk_Order_Courier" ON "Order" ("Courier_name" ASC) ;


-- -----------------------------------------------------
-- Table "OrderItem"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "OrderItem"  CASCADE;

CREATE TABLE "OrderItem" (
  "id" SERIAL ,
  "Order_id" INT NOT NULL ,
  "TicketCategory_id" INT NOT NULL ,
  "count" INT NOT NULL DEFAULT 1 ,
  PRIMARY KEY ("id") ,
  CONSTRAINT "fk_OrderItem_Order"
    FOREIGN KEY ("Order_id" )
    REFERENCES "Order" ("id" )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT "fk_OrderItem_TicketCategory"
    FOREIGN KEY ("TicketCategory_id" )
    REFERENCES "TicketCategory" ("id" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
;

CREATE INDEX "fk_OrderItem_Order" ON "OrderItem" ("Order_id" ASC) ;

CREATE INDEX "fk_OrderItem_TicketCategory" ON "OrderItem" ("TicketCategory_id" ASC) ;


-- -----------------------------------------------------
-- Table "Cart"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "Cart"  CASCADE;

CREATE TABLE "Cart" (
  "Client_User_email" VARCHAR(100) NOT NULL ,
  PRIMARY KEY ("Client_User_email") ,
  CONSTRAINT "fk_Cart_Client"
    FOREIGN KEY ("Client_User_email" )
    REFERENCES "Client" ("User_email" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
;

CREATE INDEX "fk_Cart_Client" ON "Cart" ("Client_User_email" ASC) ;


-- -----------------------------------------------------
-- Table "CartItem"
-- -----------------------------------------------------
DROP TABLE IF EXISTS "CartItem"  CASCADE;

CREATE  TABLE "CartItem" (
  "count" INT NOT NULL DEFAULT 0 ,
  "TicketCategory_id" INT NOT NULL ,
  "Cart_Client_User_email" VARCHAR(100) NOT NULL ,
  PRIMARY KEY ("TicketCategory_id", "Cart_Client_User_email") ,
  CONSTRAINT "fk_CartItem_TicketCategory"
    FOREIGN KEY ("TicketCategory_id" )
    REFERENCES "TicketCategory" ("id" )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT "fk_CartItem_Cart"
    FOREIGN KEY ("Cart_Client_User_email" )
    REFERENCES "Cart" ("Client_User_email" )
    ON DELETE CASCADE 
    ON UPDATE CASCADE)
;

CREATE INDEX "fk_CartItem_TicketCategory" ON "CartItem" ("TicketCategory_id" ASC) ;
CREATE INDEX "fk_CartItem_Cart" ON "CartItem" ("Cart_Client_User_email" ASC) ;

-- -----------------------------------------------------
-- Data for table "User"
-- -----------------------------------------------------
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('estanchev@mail.ru', '', 'Емил', 'Станчев', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('ivan@mail.bg', '', 'Иван', 'Георгиев', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('mihail@mail.bg', '', 'Михаил', 'Иванов', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('liubomir@mail.bg', '', 'Любомир', 'Веселинов', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('valentina@hardlink.eu', '', 'Валентина', 'Динкова', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('maria@mail.bg', '', 'Мария', 'Иванова', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('radko@mail.bg', '', 'Радко', 'Люцканов', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('georgi@mail.bg', '', 'Георги', 'Георгиев', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('valeri@mail.bg', '', 'Валери', 'Валентинов', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('desislava@mail.bg', '', 'Десислава', 'Иванова', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('borislav@mail.bg', '', 'Борислав', 'Бисеров', FALSE);
INSERT INTO "User" ("email", "password", "first_name", "last_name", "is_admin") VALUES ('iordan@mail.bg', '', 'Йордан', 'Иванов', FALSE);


-- -----------------------------------------------------
-- Data for table "Address"
-- -----------------------------------------------------
INSERT INTO "Address" ("id", "city", "details") VALUES (1, 'София', 'бул. Витоша 12');
INSERT INTO "Address" ("id", "city", "details") VALUES (2, 'София', 'ж.к. Овча Купел 1, бл. 411, вх. А, ап. 11');
INSERT INTO "Address" ("id", "city", "details") VALUES (3, 'София', 'ж.к. Надежда, бл. 11, вх. 3');
INSERT INTO "Address" ("id", "city", "details") VALUES (4, 'Пловдив', 'ул. Ген. Гурко 23');
INSERT INTO "Address" ("id", "city", "details") VALUES (5, 'Пловдив', 'ул. Май 11');
INSERT INTO "Address" ("id", "city", "details") VALUES (6, 'Плевен', 'ул. Св. Кирил и Методий 23');
INSERT INTO "Address" ("id", "city", "details") VALUES (7, 'Плевен', 'ул. Цар Симеон 11');
INSERT INTO "Address" ("id", "city", "details") VALUES (8, 'Варна', 'ул. Македония 3');
INSERT INTO "Address" ("id", "city", "details") VALUES (9, 'Велико Търново', 'ул. Хан Аспарух 11');
INSERT INTO "Address" ("id", "city", "details") VALUES (10, 'Бургас', 'ул. Сердика 13');
INSERT INTO "Address" ("id", "city", "details") VALUES (11, 'Стара Загора', 'пл. Берое');
INSERT INTO "Address" ("id", "city", "details") VALUES (12, 'София', 'бул. Шипченски Проход 2');
INSERT INTO "Address" ("id", "city", "details") VALUES (13, 'София', 'бул. Евлоги Георгиев');
INSERT INTO "Address" ("id", "city", "details") VALUES (14, 'София', 'кв. Сухата Река, ул. Тодорини Кукли 47');
INSERT INTO "Address" ("id", "city", "details") VALUES (15, 'София', 'ж.к. Гео Милев, ул. Манастирска карта');
INSERT INTO "Address" ("id", "city", "details") VALUES (16, 'Пловдив', 'кв. Каменица');
INSERT INTO "Address" ("id", "city", "details") VALUES (17, 'Варна', 'ул. Княз Борис I №115');
INSERT INTO "Address" ("id", "city", "details") VALUES (18, 'Велико Търново', 'бул. "Стадион Ивайло"');
INSERT INTO "Address" ("id", "city", "details") VALUES (19, 'Варна', 'ул. Никола Вaпцаров 9');
ALTER SEQUENCE "Address_id_seq" RESTART WITH 20;

-- -----------------------------------------------------
-- Data for table "Client"
-- -----------------------------------------------------
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('desislava@mail.bg', '+359 88345561', 1);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('georgi@mail.bg', '+359 89245562', 2);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('ivan@mail.bg', '+359 88345563', 3);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('mihail@mail.bg', '+359 88345564', 4);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('liubomir@mail.bg', '+359 88345565', 5);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('maria@mail.bg', '+359 88345566', 6);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('radko@mail.bg', '+359 88345567', 7);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('iordan@mail.bg', '+359 88345568', 8);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('valeri@mail.bg', '+359 88345569', 9);
INSERT INTO "Client" ("User_email", "phone_number", "Address_id") VALUES ('borislav@mail.bg', '+359 88345560', 10);


-- -----------------------------------------------------
-- Data for table "Place"
-- -----------------------------------------------------
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('стадион Берое', 11, 'Стадионът на ФК Берое се намира в Стара Загора.');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('зала Универсиада', 12, 'Зала Универсиада е първата голяма закрита многофункционална зала в България.');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('НС Васил Левски', 13, 'Националният стадион „Васил Левски“ е най-голямото спортно съоръжение с трибуни в България. Стадионът има 43 340 седящи зрителски места.');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('стадион Георги Аспарухов', 14, 'Стадион „Георги Аспарухов“ (Герена) се намира в кв. Сухата река на София. От построяването си през 1963 г. е използван от футболния и спортния клуб Левски');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('стадион Академик', 15, 'Стадион „Академик“ се намира в на границата на столичните квартали Редута и Гео Милев, близо да зала Фестивална. ');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('стадион Ботев', 16, 'Стадион „Христо Ботев“ е вторият по големина стадион в Пловдив с настоящ капацитет за 22 000 зрители');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('стадион Тича', 17, 'Стадион Тича е построен с доброволен труд от стотици почитатели на тази велика игра. Трибуните са с общ капацитет от 12 000 зрители, като северната трибуна където се помещава агитката по време на мачове побира около 4 000 души');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('стадион Ивайло', 18, 'Подробни снимки на Стадион Ивайло - в гр. Велико Търново.');
INSERT INTO "Place" ("name", "Address_id", "description") VALUES ('Дворец на културата и спорта', 19, 'Дворецът на културата и спорта във Варна e многофункционален комплекс за конгресни, културни и спортни мероприятия');


-- -----------------------------------------------------
-- Data for table "Administrator"
-- -----------------------------------------------------
INSERT INTO "Administrator" ("User_email") VALUES ('estanchev@mail.ru');
INSERT INTO "Administrator" ("User_email") VALUES ('valentina@hardlink.eu');


-- -----------------------------------------------------
-- Data for table "Sport"
-- -----------------------------------------------------
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Футбол', '1:45:00', 'Футболен мач');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Баскетбол', '1:35:00', 'Баскетболен мач');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Волейбол', '1:20:00', 'Волейболен мач');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Голф', '4:00:00', 'Голф турнир');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Мотокрос', '2:00:00', 'Мотокрос състезание');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Кънки на лед', '1:20:00', 'Кънки на лед');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Хокей', '1:00:00', 'Хокеен мач');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Бадминтон', '1:20:00', 'Мач по бадминтон');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Водна топка', '1:15:00', 'Мач по водна топка');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Тенис', '2:00:00', 'Тенис мач');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Вдигане на тежести', '2:00:00', 'Съзтезание по вдигане на тежести');
INSERT INTO "Sport" ("name", "typical_duration", "event_name") VALUES ('Лека атлетика', '3:00:00', 'Съзтезание по лека атлетика');


-- -----------------------------------------------------
-- Data for table "Event"
-- -----------------------------------------------------
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (1, 'стадион Георги Аспарухов', 'Футбол', '2010-04-29', NULL, 'ПФК Левски - ПФК Славия');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (2, 'стадион Георги Аспарухов', 'Футбол', '2010-06-1', NULL, 'ПФК Левски - ПФК ЦСКА');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (3, 'стадион Георги Аспарухов', 'Футбол', '2010-06-8', NULL, 'ПФК Левски - ПФК Литекс');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (4, 'стадион Георги Аспарухов', 'Футбол', '2010-06-15', NULL, 'ПФК Левски - ПФК Локомотив Сф');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (5, 'стадион Георги Аспарухов', 'Лека атлетика', '2010-06-17', NULL, 'Квалификация за европейско първенство 1 етап');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (6, 'стадион Георги Аспарухов', 'Лека атлетика', '2010-06-21', NULL, 'Квалификация за европейско първенство 2 етап');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (7, 'стадион Георги Аспарухов', 'Бадминтон', '2010-06-23', NULL, 'Полуфинал на държавното първенство-мъже');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (8, 'стадион Георги Аспарухов', 'Бадминтон', '2010-06-24', NULL, 'Финал на държавното първенство-мъже');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (9, 'стадион Георги Аспарухов', 'Волейбол', '2010-06-24', NULL, 'ВК Левски - ВК Академик (юноши)');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (10, 'стадион Тича', 'Футбол', '2010-06-28', NULL, 'ПФК Черно Море - ПФК Левски');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (11, 'стадион Тича', 'Футбол', '2010-07-4', NULL, 'ПФК Черно Море - ПФК Славия');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (12, 'стадион Тича', 'Футбол', '2010-07-11', NULL, 'ПФК Черно Море - ПФК Берое');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (13, 'стадион Берое', 'Футбол', '2010-07-11', NULL, 'ПФК Берое - ПФК Левски');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (14, 'стадион Берое', 'Футбол', '2010-07-18', NULL, 'ПФК Берое - ПФК Славия');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (15, 'стадион Берое', 'Футбол', '2010-07-25', NULL, 'ПФК Берое - ПФК Черно море');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (16, 'зала Универсиада', 'Баскетбол', '2010-07-26', NULL, 'БК Лукойл Академик - БК Левски, полуфинал държавно първенство (мач 1)');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (17, 'зала Универсиада', 'Баскетбол', '2010-07-29', NULL, 'БК Лукойл Академик - БК Левски, полуфинал държавно първенство (мач 2)');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (18, 'зала Универсиада', 'Вдигане на тежести', '2010-06-1', NULL, 'Държавно първенство');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (19, 'зала Универсиада', 'Баскетбол', '2010-06-1', NULL, 'БК ЦСКА - БК Лукойл Академик');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (20, 'зала Универсиада', 'Баскетбол', '2010-06-6', NULL, 'БК Лукойл Академик - БК Славия');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (21, 'Дворец на културата и спорта', 'Волейбол', '2010-06-6', NULL, 'България - Русия');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (22, 'Дворец на културата и спорта', 'Волейбол', '2010-06-12', NULL, 'България - Испания');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (23, 'Дворец на културата и спорта', 'Волейбол', '2010-06-13', NULL, 'България - Италия');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (24, 'Дворец на културата и спорта', 'Бадминтон', '2010-06-12', NULL, 'Квалификации за европейско първенство - жени');
INSERT INTO "Event" ("id", "Place_name", "Sport_name", "start", "duration", "name") VALUES (25, 'Дворец на културата и спорта', 'Бадминтон', '2010-06-13', NULL, 'Квалификации за европейско първенство - мъже');
ALTER SEQUENCE "Event_id_seq" RESTART WITH 26;
 
-- -----------------------------------------------------
-- Data for table "TicketCategory"
-- -----------------------------------------------------
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 3, 1, 200);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 5, 1, 100);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория В', 7, 1, 40);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 3, 2, 200);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 6, 2, 300);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 5, 3, 100);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 7, 3, 200);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 4, 4, 20);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 8, 4, 30);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория В', 10, 4, 50);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 11, 5, 44);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 7, 5, 0);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 5, 6, 1);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 7, 6, 5);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 4, 7, 12);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 5, 8, 34);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 2.5, 8, 11);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 10, 9, 33);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 11, 9, 3);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория В', 4, 9, 3);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 4, 10, 11);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Б', 6, 10, 1);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория В', 12, 10, 3);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория Г', 9, 10, 20);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 4, 11, 13);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 2, 12, 100);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 3, 13, 400);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 4, 14, 400);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 5, 15, 100);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 6, 16, 200);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 7, 17, 123);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 3, 18, 134);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 2, 19, 45);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 3, 20, 0);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 3, 21, 4);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 5, 21, 100);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 6, 22, 20);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 3, 23, 24);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 4, 24, 9);
INSERT INTO "TicketCategory" ("name", "price", "Event_id", "count") VALUES ('Категория А', 6, 25, 11);


-- -----------------------------------------------------
-- Data for table "Courier"
-- -----------------------------------------------------
INSERT INTO "Courier" ("name", "calculate_url") VALUES ('Спийди', 'http://www.speedy.bg/calculate/calculator1.php?language=1');
INSERT INTO "Courier" ("name", "calculate_url") VALUES ('Еконт Експрес', 'http://www.econt.com/tariff-calculator/');
INSERT INTO "Courier" ("name", "calculate_url") VALUES ('Тип Топ Куриер', 'http://www.courier.bg/index.php?option=com_wrapper&view=wrapper&Itemid=53');
INSERT INTO "Courier" ("name", "calculate_url") VALUES ('Сити Експрес', 'http://www.city-express.com/PriceCheckerServicebg.aspx');

CREATE LANGUAGE plpgsql;
CREATE OR REPLACE FUNCTION add_admin() RETURNS TRIGGER AS $new_admin$
BEGIN
    IF (NEW.is_admin = 't' AND OLD.is_admin = 'f') THEN
        INSERT INTO "Administrator" VALUES(NEW.email);
    END IF;
    RETURN NEW;
END;
$new_admin$ LANGUAGE plpgsql;
CREATE TRIGGER new_admin AFTER UPDATE OR INSERT ON "User"
	FOR EACH ROW
    EXECUTE PROCEDURE add_admin();

CREATE OR REPLACE FUNCTION rm_admin() RETURNS TRIGGER AS $rm_admin$
BEGIN
    IF (NEW.is_admin='f' AND OLD.is_admin = 't') THEN
        DELETE FROM "Administrator" WHERE "User_email"=OLD.email;
    END IF;
    RETURN OLD;
END;
$rm_admin$ LANGUAGE plpgsql;

CREATE TRIGGER rm_admin AFTER UPDATE ON "User"
	FOR EACH ROW
    EXECUTE PROCEDURE rm_admin();

CREATE OR REPLACE FUNCTION count_decrement() RETURNS TRIGGER AS $count_decrement$
BEGIN
    UPDATE "TicketCategory" AS tc SET "count"=tc."count"-NEW."count" WHERE tc."id" = NEW."TicketCategory_id";
    RETURN NEW;
END;
$count_decrement$ LANGUAGE plpgsql;

CREATE TRIGGER count_decrement AFTER INSERT ON "OrderItem"
	FOR EACH ROW
    EXECUTE PROCEDURE count_decrement();

CREATE OR REPLACE FUNCTION default_duration() RETURNS TRIGGER AS $default_duration$
BEGIN
    IF (NEW.duration = NULL) THEN
        NEW.duration = (SELECT typical_duration FROM "Sport" where name=NEW."Sport_name");
    END IF;
    RETURN OLD;
END;
$default_duration$ LANGUAGE plpgsql;

CREATE TRIGGER set_duration AFTER UPDATE OR INSERT  ON "Event"
    FOR EACH ROW
    EXECUTE PROCEDURE default_duration();

CREATE FUNCTION monthly_orders(integer) RETURNS bigint
    AS 'select count(*) from "Order" where date_part(''month'', "created_at") = $1;'
    LANGUAGE SQL
    RETURNS NULL ON NULL INPUT;

CREATE FUNCTION monthly_tickets(integer) RETURNS bigint
    AS 'select sum("count") from "OrderItem" JOIN "Order" on "Order_id"="Order"."id" where date_part(''month'', "created_at") = $1;'
    LANGUAGE SQL
    RETURNS NULL ON NULL INPUT;

CREATE VIEW upcoming_events AS SELECT * From "Event" where "Event"."start">'now' ORDER BY "start" ASC;
CREATE VIEW past_events AS SELECT * From "Event" where "Event"."start"<='now' ORDER BY "start" DESC;
