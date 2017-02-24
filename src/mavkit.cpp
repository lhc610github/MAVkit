#include <mavkit/MavlinkUDP.h>
#include <mavkit/MavlinkTCP.h>
#include <mavkit/MavlinkSerial.h>
#include <mavkit/MavlinkLogReader.h>
#include <mavkit/MavlinkLogWriter.h>
#include <mavkit/MavlinkDisplay.h>
#include <iostream>
#include <unistd.h>
#include <argp.h>

MavMessengerInterface* master = NULL;
std::vector<MavMessengerInterface*> outputs;

//----------------------------------------------------------------------------//
void usage()
{
    std::cout << "usage: mavkit master output1 output2 ..." << std::endl;
    std::cout << "  master/output: IP address, tty path or log file." << std::endl;
}
//----------------------------------------------------------------------------//
void add_messenger(MavMessengerInterface* messenger)
{
    if(master == NULL)
    {
        master = messenger;
    }
    else
    {
        outputs.push_back(messenger);
        master->append_listener(messenger);
        messenger->append_listener(master);
    }
}
//----------------------------------------------------------------------------//
bool isdigit(char *str)
{
    char c = str[0];
    int i = 1;
    while(c != '\0')
    {
        if(!isdigit(c))
        {
            return false;
        }
        c = str[i];
        i++;
    }
    return i > 1;
}
//----------------------------------------------------------------------------//
int parse_messenger_type(char *arg)
{
    if(strcmp(arg, "--tty") == 0)
        return 0;
    if(strcmp(arg, "--udp_client") == 0)
        return 1;
    if(strcmp(arg, "--udp_server") == 0)
        return 2;
    if(strcmp(arg, "--tcp_client") == 0)
        return 3;
    if(strcmp(arg, "--tcp_server") == 0)
        return 4;
    if(strcmp(arg, "--file") == 0)
        return 5;
    if(strcmp(arg, "--log") == 0)
        return 6;
    if(strcmp(arg, "--display") == 0)
        return 7;
    return -1;
}
//----------------------------------------------------------------------------//
char* get_next_arg(int &index, int argc, char** argv)
{
    if(index < argc)
        return argv[index++];

    return NULL;
}
//----------------------------------------------------------------------------//
bool create_tty(int &index, int argc, char** argv)
{
    char* baudrate_str = get_next_arg(index, argc, argv);
    if(baudrate_str == NULL)
    {
        //TODO usage
        std::cout << "baudrate fail" << std::endl;
        return 0;
    }
    char* device_str = get_next_arg(index, argc, argv);
    if(device_str == NULL)
    {
        //TODO usage
        std::cout << "device fail" << std::endl;
        return 0;
    }

    if(isdigit(baudrate_str))
    {
        add_messenger(new MavlinkSerial(device_str, atoi(baudrate_str)));
        return true;
    }
    else
    {
        //TODO usage
        std::cout << "baudrate not a number" << std::endl;
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_udp_client(int &index, int argc, char** argv)
{
    char* ip_str = get_next_arg(index, argc, argv);
    if(ip_str == NULL)
    {
        //TODO usage
        std::cout << "ip fail" << std::endl;
        return 0;
    }
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        //TODO usage
        std::cout << "port fail" << std::endl;
        return 0;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkUDP(ip_str, atoi(port_str)));
        return true;
    }
    else
    {
        //TODO usage
        std::cout << "port not a number" << std::endl;
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_udp_server(int &index, int argc, char** argv)
{
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        //TODO usage
        std::cout << "port fail" << std::endl;
        return 0;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkUDP(atoi(port_str)));
        return true;
    }
    else
    {
        //TODO usage
        std::cout << "port not a number" << std::endl;
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_tcp_client(int &index, int argc, char** argv)
{
    char* ip_str = get_next_arg(index, argc, argv);
    if(ip_str == NULL)
    {
        //TODO usage
        std::cout << "ip fail" << std::endl;
        return 0;
    }
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        //TODO usage
        std::cout << "port fail" << std::endl;
        return 0;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkTCP(ip_str, atoi(port_str)));
        return true;
    }
    else
    {
        //TODO usage
        std::cout << "port not a number" << std::endl;
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_tcp_server(int &index, int argc, char** argv)
{
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        //TODO usage
        std::cout << "port fail" << std::endl;
        return 0;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkTCP(atoi(port_str)));
        return true;
    }
    else
    {
        //TODO usage
        std::cout << "port not a number" << std::endl;
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_file(int &index, int argc, char** argv)
{
    char* file_str = get_next_arg(index, argc, argv);
    if(file_str == NULL)
    {
        //TODO usage
        std::cout << "file fail" << std::endl;
        return 0;
    }

    char* speed_str = get_next_arg(index, argc, argv);
    if(speed_str == NULL)
    {
        //TODO usage
        std::cout << "speed fail" << std::endl;
        return 0;
    }

    char* start_str = get_next_arg(index, argc, argv);
    if(start_str == NULL)
    {
        //TODO usage
        std::cout << "start fail" << std::endl;
        return 0;
    }

    add_messenger(new MavlinkLogReader(file_str, atof(speed_str), atoi(start_str)));
    return true;
}
//----------------------------------------------------------------------------//
bool create_log(int &index, int argc, char** argv)
{
    add_messenger(new MavlinkLogWriter("/log"));
    return true;
}
//----------------------------------------------------------------------------//
bool create_display(int &index, int argc, char** argv)
{
    add_messenger(new MavlinkDisplay());
    return true;
}
//----------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        std::cout << "no args" << std::endl;
        //TODO usage
        return 0;
    }

    int arg_i = 1;
    while(arg_i < argc)
    {
        char* messenger_str = get_next_arg(arg_i, argc, argv);
        int messenger_type = parse_messenger_type(messenger_str);

        if(messenger_type == -1)
        {
            std::cout << "bad messenger type" << std::endl;
            //TODO usage
            return 0;
        }

        switch (messenger_type)
        {
            case 0:
                create_tty(arg_i, argc, argv);
                break;
            case 1:
                create_udp_client(arg_i, argc, argv);
                break;
            case 2:
                create_udp_server(arg_i, argc, argv);
                break;
            case 3:
                create_tcp_client(arg_i, argc, argv);
                break;
            case 4:
                create_tcp_server(arg_i, argc, argv);
                break;
            case 5:
                create_file(arg_i, argc, argv);
                break;
            case 6:
                create_log(arg_i, argc, argv);
                break;
            case 7:
                create_display(arg_i, argc, argv);
                break;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------//
// int main(int argc, char* argv[])
// {
//     int c;
//     while (true)
//     {
//         static struct option long_options[] =
//         {
//             /* These options set a flag. */
//             //   {"verbose", no_argument,       &verbose_flag, 1},
//             //   {"brief",   no_argument,       &verbose_flag, 0},
//             /* These options don’t set a flag.
//             We distinguish them by their indices. */
//             {"tty",        required_argument, 0, 'a'},
//             {"udp_client", required_argument, 0, 'b'},
//             {"udp_server", required_argument, 0, 'c'},
//             {"tcp_client", required_argument, 0, 'd'},
//             {"tcp_server", required_argument, 0, 'e'},
//             {"file",       required_argument, 0, 'f'},
//             {"log",        no_argument,       0, 'g'},
//             {"display",    no_argument,       0, 'h'},
//             {0, 0, 0, 0}
//         };
//
//         /* getopt_long stores the option index here. */
//         int option_index = 0;
//         c = getopt_long (argc, argv, "abcdef", long_options, &option_index);
//
//         /* Detect the end of the options. */
//         if (c == -1)
//             break;
//
//         switch (c)
//         {
//             // case 0:
//             // {
//             //     /* If this option set a flag, do nothing else now. */
//             //     if (long_options[option_index].flag != 0)
//             //     break;
//             //     printf ("option %s", long_options[option_index].name);
//             //     if (optarg)
//             //     printf (" with arg %s", optarg);
//             //     printf ("\n");
//             //     break;
//             // }
//             case 'a':
//             {
//                 if(optind + 1 > argc)
//                 {
//                     std::cout << "./mavkit: option \'--tty\' requires two arguments" << std::endl;
//                     exit(0);
//                 }
//
//                 char* device   = argv[--optind];
//                 char* baudrate_str = argv[++optind];
//                 optind++;
//                 if(isdigit(baudrate_str))
//                 {
//                     add_messenger(new MavlinkSerial(device, atoi(baudrate_str)));
//                 }
//                 else
//                 {
//                     //TODO baudrate not a number
//                     usage();
//                     exit(0);
//                 }
//                 break;
//             }
//             case 'b':
//             {
//                 if(optind + 1 > argc)
//                 {
//                     std::cout << "./mavkit: option \'--udp_client\' requires two arguments" << std::endl;
//                     exit(0);
//                 }
//
//                 char* ip       = argv[--optind];
//                 char* port_str = argv[++optind];
//                 optind++;
//
//                 if(isdigit(port_str))
//                 {
//                     add_messenger(new MavlinkUDP(ip, atoi(port_str)));
//                 }
//                 else
//                 {
//                     std::cout << "./mavkit: option \'--udp_client\' port is not a number" << std::endl;
//                     exit(0);
//                 }
//                 break;
//             }
//             case 'c':
//             {
//                 if(isdigit(optarg))
//                 {
//                     add_messenger(new MavlinkUDP(atoi(optarg)));
//                 }
//                 else
//                 {
//                     std::cout << "./mavkit: option \'--udp_server\' port is not a number" << std::endl;
//                     exit(0);
//                 }
//                 break;
//             }
//             case 'd':
//             {
//                 if(optind + 1 > argc)
//                 {
//                     std::cout << "./mavkit: option \'--udp_client\' requires two arguments" << std::endl;
//                     exit(0);
//                 }
//
//                 char* ip       = argv[--optind];
//                 char* port_str = argv[++optind];
//                 optind++;
//
//                 if(isdigit(port_str))
//                 {
//                     add_messenger(new MavlinkTCP(ip, atoi(port_str)));
//                 }
//                 else
//                 {
//                     std::cout << "./mavkit: option \'--udp_client\' port is not a number" << std::endl;
//                     exit(0);
//                 }
//                 break;
//             }
//             case 'e':
//             {
//                 if(isdigit(optarg))
//                 {
//                     add_messenger(new MavlinkTCP(atoi(optarg)));
//                 }
//                 else
//                 {
//                     std::cout << "./mavkit: option \'--udp_server\' port is not a number" << std::endl;
//                     exit(0);
//                 }
//                 break;
//             }
//             case 'f':
//             {
//                 add_messenger(new MavlinkLogReader(optarg, 1.0, 0)); // speed_multiplier and start time
//                 break;
//             }
//             case 'g':
//             {
//                 add_messenger(new MavlinkLogWriter("log/"));
//                 break;
//             }
//             case 'h':
//             {
//                 add_messenger(new MavlinkDisplay());
//                 break;
//             }
//             default:
//               exit(0);
//         }
//     }
//
//       /* Instead of reporting ‘--verbose’
//          and ‘--brief’ as they are encountered,
//          we report the final status resulting from them. */
//     //   if (verbose_flag)
//     //     puts ("verbose flag is set");
//
//     /* Print any remaining command line arguments (not options). */
//     if (optind < argc)
//     {
//         printf ("non-option ARGV-elements: ");
//         while (optind < argc)
//         printf ("%s ", argv[optind++]);
//         putchar ('\n');
//     }
//
//     if(master == NULL)
//     {
//         std::cerr << "Need at least a master messenger interface." << std::endl;
//         exit(0);
//     }
//
//     master->start();
//     for(int i=0;i<outputs.size();i++)
//         outputs[i]->start();
//
//     //send Mavlink2 heartbeat to force protocol version
//     while(true)
//     {
//         //MSG1
//         mavlink_message_t msg;
//         mavlink_msg_heartbeat_pack(1, 2, &msg, MAV_TYPE_SUBMARINE, MAV_AUTOPILOT_GENERIC, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
//         master->send_message(msg);
//         usleep(1000000);
//     }
//     // master->join();
//
//     return 0;
// }
//----------------------------------------------------------------------------//
