<%
    var Settings = {
        /* Used as a salt in the hashing of passwords */
        secret: "12KJH1*&^!*^@fHIAUSDYF(&WA*y1231hy23i12bL*Y(8y132`23h21kj",

        field_names: {
            email: "Електронен адрес",
            password: "Парола в хеширан вид",
            first_name: "Собствено име",
            last_name: "Фамилно име",
            is_admin: "Администратор?",
            name: "Име",
            calculate_url: "Адрес за проверка на тарифи",
            typical_duration: "Типична продължителност в минути",
            event_name: "Име на проява",
            phone_number: "Телефонен номер за връзка",
            start: "Начало",
            duration: "Продължителност",
            description: "Подробности",

            month: "Месец",
            year: "Година",
            day: "Ден",
            hour: "Час",
            minute: "Минути"
        },

        /* Chose years from min_year, year_range in forms */
        min_year: 2009,
        year_offset: 5


    };
%>
