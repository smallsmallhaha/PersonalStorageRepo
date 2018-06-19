create database  if not exists `jwgl`;
use `jwgl`;

drop table if exists `hactcmjwgl2016b`;
create table `hactcmjwgl2016b` (
  `CourseName` varchar(50) default null,
  `CourseId` varchar(20) default null,
  `StuId` int(20) default null,
  `StuName` varchar(40) default null,
  `ClassName` varchar(40) default null
) engine=innodb default charset=utf8;

load data local infile '/home/kekliu/Documents/python/fuck-login/mycrawler/jwgl/students.txt'
into table hactcmjwgl2016b
fields terminated by ',';

select count(*) from hactcmjwgl2016b;
