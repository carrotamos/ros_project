//现在让我们分解代码，跳过上一篇教程中解释的内容。产生的整体效果是旋转螺旋线，每个顶点都有向上的线条。

//ros/ros.h 是一个实用的头文件，它引用了 ROS 系统中大部分常用的头文件。 
#include <ros/ros.h>//
#include <visualization_msgs/Marker.h>

#include <cmath>

int main( int argc, char** argv )
{
//初始化 ROS 。它允许 ROS 通过命令行进行名称重映射——然而这并不是现在讨论的重点。在这里，我们也可以指定节点的名称——运行过程中，节点的名称必须唯一。

//这里的名称必须是一个 base name ，也就是说，名称内不能包含 / 等符号。

  ros::init(argc, argv, "points_and_lines");
//为这个进程的节点创建一个句柄。第一个创建的 NodeHandle 会为节点进行初始化，最后一个销毁的 NodeHandle 则会释放该节点所占用的所有资源。  
 ros::NodeHandle n;

//告诉 master 我们将要在 point_and_lines（话题名） 上发布<visualization_msgs::Marker> 消息类型的消息。这样 master 就会告诉所有订阅了 chatter 话题的节点，将要有数据发布。第二个参数是发布序列的大小。如果我们发布的消息的频率太高，缓冲区中的消息在大于 10 个的时候就会开始丢弃先前发布的消息。

//NodeHandle::advertise() 返回一个 ros::Publisher 对象,它有两个作用： 1) 它有一个 publish() 成员函数可以让你在topic上发布消息； 2) 如果消息类型不对,它会拒绝发布。 
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
//ros::Rate 对象可以允许你指定自循环的频率。它会追踪记录自上一次调用 Rate::sleep() 后时间的流逝，并休眠直到一个频率周期的时间。 

  ros::Rate r(30);

  float f = 0.0;
  while (ros::ok())
  {
//在这里，我们创建三个visualization_msgs / Marker消息并初始化它们的所有共享数据。 我们利用消息成员默认为0并且仅设置姿势的w成员这一事实。
    visualization_msgs::Marker points, line_strip, line_list;
    points.header.frame_id = line_strip.header.frame_id = line_list.header.frame_id = "/my_frame";
    points.header.stamp = line_strip.header.stamp = line_list.header.stamp = ros::Time::now();
    points.ns = line_strip.ns = line_list.ns = "points_and_lines";
    points.action = line_strip.action = line_list.action = visualization_msgs::Marker::ADD;
    points.pose.orientation.w = line_strip.pose.orientation.w = line_list.pose.orientation.w = 1.0;


//我们为三个标记分配了三个不同的ID。 使用points_and_lines命名空间可确保它们不会与其他广播公司发生冲突。
    points.id = 0;
    line_strip.id = 1;
    line_list.id = 2;

//在这里，我们将标记类型设置为POINTS，LINE_STRIP和LINE_LIST。

    points.type = visualization_msgs::Marker::POINTS;
    line_strip.type = visualization_msgs::Marker::LINE_STRIP;
    line_list.type = visualization_msgs::Marker::LINE_LIST;


//POINTS标记分别使用x和y成员的宽度和高度
    points.scale.x = 0.2;
    points.scale.y = 0.2;


//而LINE_STRIP和LINE_LIST标记仅使用x组件，它定义线宽。 比例值以米为单位。
    line_strip.scale.x = 0.1;
    line_list.scale.x = 0.1;

//在这里，我们将点设置为绿色，将线条设置为蓝色，将线列表设置为红色。

    // Points are green
    points.color.g = 1.0f;
    points.color.a = 1.0;

    // Line strip is blue
    line_strip.color.b = 1.0;
    line_strip.color.a = 1.0;

    // Line list is red
    line_list.color.r = 1.0;
    line_list.color.a = 1.0;


//我们使用正弦和余弦来生成螺旋线。 POINTS和LINE_STRIP标记都只需要每个顶点一个点，而LINE_LIST标记需要2个。
// 为点和线创建顶点
    for (uint32_t i = 0; i < 100; ++i)
    {
      float y = 5 * sin(f + i / 100.0f * 2 * M_PI);
      float z = 5 * cos(f + i / 100.0f * 2 * M_PI);

      geometry_msgs::Point p;
      p.x = (int32_t)i - 50;
      p.y = y;
      p.z = z;

      points.points.push_back(p);
      line_strip.points.push_back(p);

////每行的行列表需要两个点
      line_list.points.push_back(p);
      p.z += 1.0;
      line_list.points.push_back(p);
    }


    marker_pub.publish(points);
    marker_pub.publish(line_strip);
    marker_pub.publish(line_list);

    r.sleep();

    f += 0.04;
  }
}
