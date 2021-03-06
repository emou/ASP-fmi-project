<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
                      "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="bg" lang="bg">
    <head>
        <title> Продажба на билети за спортни събития :: Администраторска част :: <%= blocks.title||"Начална страница" %> </title>
        <meta http-equiv="content-type" content="text/html;charset=utf-8" />
        <meta http-equiv="Content-Style-Type" content="text/css" />
        <meta name="Copyright" content="Copyright (c) Emil Stanchev" />
        <meta name="keywords" content="спорт, билети, продажба" />
        <meta name="description" content="Продажба на билети за спортни прояви" />

        <link rel="stylesheet" href="/media/sport.css" type="text/css" media="all" />
        <link rel="stylesheet" href="/media/admin.css" type="text/css" media="all" />
    </head>

    <body>
        <div id="logged">
            <%= logged %>
        </div>
        <h1 id="admin_header"><a href="/admin/default.asp">Администраторска част</a></h1>
        <h2 id="header"><a href="/default.asp">Онлайн спортни билети</a></h2>

        <div id="content">
            <%= flash %>
            <%= blocks['content'] %>
        </div>
        <!-- end content -->

        <div id="footer">Проект по ASP програмиране. Емил Станчев, ф.н. 71100, юли 2010г.</div>
    </body>
</html>
