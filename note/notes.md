1. android layout

Dimension
A dimension value defined in XML. A dimension is specified with a number followed by a unit of measure. For example: 10px, 2in, 5sp. The following units of measure are supported by Android:

dp
Density-independent Pixels - An abstract unit that is based on the physical density of the screen. These units are relative to a 160 dpi (dots per inch) screen, on which 1dp is roughly equal to 1px. When running on a higher density screen, the number of pixels used to draw 1dp is scaled up by a factor appropriate for the screen's dpi. Likewise, when on a lower density screen, the number of pixels used for 1dp is scaled down. The ratio of dp-to-pixel will change with the screen density, but not necessarily in direct proportion. Using dp units (instead of px units) is a simple solution to making the view dimensions in your layout resize properly for different screen densities. In other words, it provides consistency for the real-world sizes of your UI elements across different devices.
sp
Scale-independent Pixels - This is like the dp unit, but it is also scaled by the user's font size preference. It is recommend you use this unit when specifying font sizes, so they will be adjusted for both the screen density and the user's preference.
pt
Points - 1/72 of an inch based on the physical size of the screen, assuming a 72dpi density screen.
px
Pixels - Corresponds to actual pixels on the screen. This unit of measure is not recommended because the actual representation can vary across devices; each devices may have a different number of pixels per inch and may have more or fewer total pixels available on the screen.
mm
Millimeters - Based on the physical size of the screen.
in
Inches - Based on the physical size of the screen.
Note: A dimension is a simple resource that is referenced using the value provided in the name attribute (not the name of the XML file). As such, you can combine dimension resources with other simple resources in the one XML file, under one <resources> element.

FILE LOCATION:
res/values/filename.xml
The filename is arbitrary. The <dimen> element's name will be used as the resource ID.
RESOURCE REFERENCE:
In Java: R.dimen.dimension_name
In XML: @[package:]dimen/dimension_name
SYNTAX:
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <dimen
        name="dimension_name"
        >dimension</dimen>
</resources>
ELEMENTS:
<resources>
Required. This must be the root node.
No attributes.

<dimen>
A dimension, represented by a float, followed by a unit of measurement (dp, sp, pt, px, mm, in), as described above.
attributes:

name
String. A name for the dimension. This will be used as the resource ID.
EXAMPLE:
XML file saved at res/values/dimens.xml:
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <dimen name="textview_height">25dp</dimen>
    <dimen name="textview_width">150dp</dimen>
    <dimen name="ball_radius">30dp</dimen>
    <dimen name="font_size">16sp</dimen>
</resources>
This application code retrieves a dimension:

Resources res = getResources();
float fontSize = res.getDimension(R.dimen.font_size);
This layout XML applies dimensions to attributes:

<TextView
    android:layout_height="@dimen/textview_height"
    android:layout_width="@dimen/textview_width"
    android:textSize="@dimen/font_size"/>


2. 不要在异步线程中创建或者销毁ProgressDialog


3. 异步加载图片
public class DownLoadImage extends AsyncTask<String, Void, Bitmap> {
    ImageView imageView;

    DownLoadImage(ImageView imageView) {
        this.imageView = imageView;
    }

    @Override
    protected Bitmap doInBackground(String... urls) {

        String url = urls[0];
        Bitmap tmpBitmap = null;
        try {
            InputStream is = new java.net.URL(url).openStream();
            tmpBitmap = BitmapFactory.decodeStream(is);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return tmpBitmap;
    }

    @Override
    protected void onPostExecute(Bitmap result) {
        imageView.setImageBitmap(result);
    }
}

new DownLoadImage(v).execute("https://ss0.bdstatic.com/5aV1bjqh_Q23odCf/static" +
                "/superman/img/logo/bd_logo1_31bdc765.png");


4. mysql 字符集更改
在 /etc/mysql/my.cnf中添加
[mysqld]
character-set-server=utf8


5. 
ERROR 1290 (HY000): The MySQL server is running with the --secure-file-priv option so it cannot execute this statement
Solution:
For Ubuntu, edit the file /etc/mysql/mysql.conf.d/mysqld.cnf and add the following line at the end:
secure_file_priv=""


6.
ERROR 29 (HY000): File '/home/kekliu/students.csv' not found (Errcode: 13 - Permission denied)
solution:
load data local infile '/home/kekliu/student.csv' into table jwgl2016b;




sed的高级用法:
d 清空模式空间,开始下一个循环
D 若模式空间有新行,删除模式空间第一行,使用剩余的模式空间开始下一个循环而非读取文件;若无新行,直接退出

g 使用保持空间替换模式空间
G 将保持空间正文全部追加到模式空间

h 使用模式空间替换保持空间
H 将模式空间正文全部追加到保持空间

n 若自动打印没有被关闭,打印模式空间,读取一行,填充整个模式空间,若无新行,则退出sed
N 追加一行到模式空间,若无新行,则退出sed

p 打印模式空间
P 打印模式空间第一行



If pattern space contains newlines, delete text in the pattern space up to the first newline, and restart cycle with the resultant pattern space, without reading a new line of input.

If pattern space contains no newline, start a normal new cycle as if the d command was issued.



