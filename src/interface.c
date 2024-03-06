/*  interface.c
* 
*	GP—Bullet
*	网络2101高鹏
*   2023.12.10
*/


#include <stdio.h>
#include <gtk/gtk.h>
#include "forward.h"
#include "readPcap.h"
#include "readRoute.h"

static GtkWidget *pcapFile;
static GtkWidget *routeFile;
GtkWidget *file;

char fileRoute[1024];
char filePcap[1024];

char *copyStr(char *dest, const gchar *src)
{
    if (src == NULL) {
        return NULL;
    }
    char *strDest = dest;
    while ((*dest++ = *src++) != '\0');
    return strDest;
}

// Clicking different buttons triggers different actions
void on_button_clicked(GtkWidget *button, gpointer data)
{
    if ((int)data == 1) {
        gtk_entry_set_text(GTK_ENTRY(pcapFile), "");
        gtk_entry_set_text(GTK_ENTRY(routeFile), "");
    } else if ((int)data == 2) {
        const gchar *pcapName = gtk_entry_get_text(GTK_ENTRY(pcapFile));
        const gchar *routeName = gtk_entry_get_text(GTK_ENTRY(routeFile));
        g_print("Reading data: %s\n", pcapName);
        g_print("file-name: %s\n", pcapName);
        g_print("route-file-name: %s\n", routeName);

        g_print("Copying strings\n");
        copyStr(fileRoute, routeName);
        copyStr(filePcap, pcapName);

        printf("fileRoute: %s, filePcap: %s\n", fileRoute, filePcap);

        read_Route(fileRoute);
        read_pcap(filePcap);
    }
}

void file_OK(GtkWidget *w, gpointer *data)
{
    gtk_entry_set_text(GTK_ENTRY(data), gtk_file_selection_get_filename(GTK_FILE_SELECTION(file)));

    gtk_widget_destroy(file);
}

// File selection
void select_file(GtkWidget *w, gpointer *data)
{
    file = gtk_file_selection_new("File selection");

    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(file)->ok_button), "clicked", G_CALLBACK(file_OK), data);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(file)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show(file);
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *sep;

    gtk_init(&argc, &argv);
    gtk_settings_set_string_property(gtk_settings_get_default(), "gtk-theme-name", "Mist", "Mist");

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "IPv4 Forwarding Simulation");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    table = gtk_table_new(4, 8, TRUE);
    gtk_container_add(GTK_CONTAINER(window), table);

    label = gtk_label_new("Select files to perform packet forwarding");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 3, 5, 0, 1);

    // Pcap file path selection
    label = gtk_label_new("Pcap file path:");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 2, 1, 2);

    pcapFile = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), pcapFile, 2, 6, 1, 2);
    button = gtk_button_new_with_label("Select file");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(select_file), pcapFile);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 6, 7, 1, 2);

    // Route table file path selection
    label = gtk_label_new("Route table file path:");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 2, 2, 3);

    routeFile = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), routeFile, 2, 6, 2, 3);
    button = gtk_button_new_with_label("Select file");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(select_file), routeFile);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 6, 7, 2, 3);

    button = gtk_button_new_with_label("Clear");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), (gpointer)1);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 3, 3, 4);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Confirm");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), (gpointer)2);
    g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 5, 7, 3, 4);
    gtk_widget_show(button);

    gtk_widget_show_all(window);
    gtk_main();
    return FALSE;
}