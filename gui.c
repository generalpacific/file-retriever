
#include"gui.h"

#define N_COL 1
#define SUCCESS 1
#define FAIL -1
#define MAXBIG 1000

#define X_SEARCH 300
#define Y_UPPER 15
#define Y_LOWER 70

#define X_INDEX 15

// search part
static GtkWidget *searchButton;
static GtkWidget *list;
static GtkWidget *queryEntry;
static char *query;

// index part	
static GtkWidget *indexButton;
static GtkWidget *textView;
static GtkTextBuffer *textViewBuffer;

void readFile(char *filename, char *buf){
	int i = 0;
	char ch;


	FILE *fp;
	fp = fopen(filename,"r");

	while(1){
		ch = fgetc(fp);
		if(ch == EOF){
			break;
		}
		buf[i++] = ch;
	}
	buf[i] = '\0';
}

int readLineFromFile(FILE *fp, char *retLine){
	int i = 0;

	char ch;

//	printf("in getline\n");

	while(1) {
		ch = fgetc(fp);
		if(ch == EOF){
			return FAIL;
		}
		if(ch == '\n')
			break;
		retLine[i++] = ch;
		if(i == MAXBIG){
			break;
		}

	}
	
	retLine[i] = '\0';

	return SUCCESS;

}

static void indexFunc(GtkWidget *widget, gpointer *data){
	char buf[1000];
	char *op;
	int lineNo = 0,dots;
	GtkTextIter endIter,startIter;
	GError **error;

	struct stat sb;
	struct passwd *userinfo;

	int pid;
	int status;

	gtk_widget_set_sensitive(indexButton,FALSE); 
	gtk_widget_set_sensitive(searchButton,FALSE); 
	
	sprintf(buf,"Deleting Previous Index...\n\n");
	gtk_text_buffer_set_text(textViewBuffer,buf,-1);
	lineNo += 2;

	system("./deleteIndex.sh");

	sprintf(buf,"Indexing started.\n");
	gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
	gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
	++lineNo;

	getlogin_r(buf,1000);
	userinfo = getpwnam(buf);

	sprintf(buf,"./crawler %s",userinfo->pw_dir);

	g_spawn_command_line_async(buf,error);

	dots = 0;
	sprintf(buf,"Crawling\n");
	++lineNo;
	gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
	gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
	while(stat("INDEX/donecrawl",&sb) == -1){
		sprintf(buf,"#");
		++dots;
		gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
		gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
		while(gtk_events_pending()){
			gtk_main_iteration();
		}
		if(dots == 20){
			gtk_text_buffer_get_iter_at_line(textViewBuffer, &startIter, lineNo+1);
			gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
			gtk_text_buffer_delete(textViewBuffer, &startIter, &endIter);
			gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
			gtk_text_buffer_insert(textViewBuffer, &endIter, "",-1);
			dots=0;

		}
		sleep(1);
	}


	sprintf(buf,"\nCrawling Done\n\n");
	lineNo += 3;
	gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
	gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
	
	sprintf(buf,"./index\n");

	g_spawn_command_line_async(buf,error);

	dots = 0;
	sprintf(buf,"Indexing\n");
	++lineNo;
	gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
	gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
	while(stat("INDEX/STATUS.txt",&sb) == -1){
		sprintf(buf,"#");
		++dots;
		gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
		gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
		while(gtk_events_pending()){
			gtk_main_iteration();
		}
		if(dots == 20){
			gtk_text_buffer_get_iter_at_line(textViewBuffer, &startIter, lineNo+1);
			gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
			gtk_text_buffer_delete(textViewBuffer, &startIter, &endIter);
			gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
			gtk_text_buffer_insert(textViewBuffer, &endIter, "",-1);
			dots=0;

		}
		sleep(1);
	}

	sprintf(buf,"\nIndexing Done.\n");
	gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
	gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
	
	sprintf(buf,"\n==================\n");
	gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
	gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
	
	readFile("INDEX/STATUS.txt",buf);
	gtk_text_buffer_get_end_iter(textViewBuffer, &endIter);
	gtk_text_buffer_insert(textViewBuffer,&endIter,buf,-1);
	

	GtkWidget *dialog;

	dialog = gtk_message_dialog_new((GtkWindow*)data, GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Indexing Complete","title");
	gtk_window_set_title(GTK_WINDOW(dialog),"Information");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	
	gtk_widget_set_sensitive(indexButton,TRUE); 
	gtk_widget_set_sensitive(searchButton,TRUE); 


}

static void openFileOnClick(GtkTreeView *model, GtkTreePath *path, gpointer *data){
	char *fileName,*treePath;
	char cmd[MAXBIG];
	GtkListStore *store;
	GtkTreeIter iter;
	

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(model)));
	
	gtk_tree_model_get_iter(GTK_TREE_MODEL(store),&iter,path);
	gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &fileName,-1);

	sprintf(cmd,"gnome-open \"%s\"",fileName);
	printf("CMD == %s\n",cmd);
	system(cmd);

	treePath = gtk_tree_path_to_string(path);

	g_free(treePath);
	g_free(fileName);
}

static void list_add_cb(GtkWidget *widget,gpointer *data){
	GtkListStore *store;
	GtkTreeIter iter;
	char result[MAXBIG];
	char cmd[100];
	int noResultFlag;
	
	query = (char*)gtk_entry_get_text(GTK_ENTRY(queryEntry));
	sprintf(cmd,"./search \"%s\"\n",query);
	system(cmd);
	
	FILE *fp;
	fp = fopen("SearchResults.txt","r");

	noResultFlag = FAIL;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_clear(GTK_LIST_STORE(store));
	while(readLineFromFile(fp,result) != FAIL){
		noResultFlag = SUCCESS;
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,0,result,-1);
	}
	
	if(noResultFlag == FAIL){
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new((GtkWindow*)data, GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"No Results Found!!","title");
		gtk_window_set_title(GTK_WINDOW(dialog),"Information");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}
}

static GtkWidget* setup_list(){
	GtkWidget *s_win;
	GtkListStore *store;
	GtkCellRenderer *cell;
	GtkTreeViewColumn *column;
	
	s_win = gtk_scrolled_window_new(NULL,NULL);
	store = gtk_list_store_new(N_COL,G_TYPE_STRING);
	
	list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	
	cell = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Results",cell,"text",0,NULL);
	
	gtk_tree_view_append_column(GTK_TREE_VIEW(list),column);
	
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(s_win), GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	
	gtk_container_add(GTK_CONTAINER(s_win),list);
	
	g_signal_connect(list,"row-activated",G_CALLBACK(openFileOnClick),NULL);
	
	g_object_unref(G_OBJECT(store));
	
	return s_win;
	
}

int main(int argc, char **argv){
	GtkWidget *window;

	int ret,indexFlag;
	char cmd[50];
	struct stat sb;
	ret = stat("INDEX/STATUS.txt",&sb);
	
	g_thread_init (NULL);
	gdk_threads_init ();
	gdk_threads_enter();

	if(ret == -1){
		indexFlag = FAIL;
	}
	



	gtk_init(&argc,&argv);

	// Window initiation
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
//	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 700);
	gtk_window_set_title(GTK_WINDOW(window), "Search");
	gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	gtk_window_maximize(GTK_WINDOW(window));
	GTK_WINDOW(window)->allow_shrink = FALSE;

	// SEARCH PART

	//Widgets - Entry and a button

	
	GtkWidget *fixed;

	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window),fixed);


	queryEntry = gtk_entry_new();
	gtk_widget_set_size_request(queryEntry, 850, 30);


	searchButton = gtk_button_new_with_label("Search");  
	gtk_widget_set_size_request(searchButton, 80, 30);



	gtk_fixed_put(GTK_FIXED(fixed),queryEntry,X_SEARCH,Y_UPPER);
	gtk_fixed_put(GTK_FIXED(fixed),searchButton,X_SEARCH + 870,Y_UPPER);

			
	g_signal_connect(searchButton,"clicked",G_CALLBACK(list_add_cb),NULL);

	//  result
	
	GtkWidget *view_list;
	
	view_list = setup_list();
	
	gtk_widget_set_size_request(view_list,1000,600);
	
	gtk_fixed_put(GTK_FIXED(fixed),view_list,X_SEARCH,Y_LOWER);

		
	// INDEX PART

	// The index Button

	indexButton = gtk_button_new_with_label("Index");
	gtk_widget_set_size_request(indexButton,80,30);

	gtk_fixed_put(GTK_FIXED(fixed), indexButton, X_INDEX,Y_UPPER);
	g_signal_connect(indexButton,"clicked",G_CALLBACK(indexFunc),NULL);

	//The text view for updates and status

	GtkWidget *label;
	char buf[MAXBIG];

	label = gtk_label_new("Index Status : ");
	gtk_fixed_put(GTK_FIXED(fixed),label,X_INDEX, Y_LOWER);


	textView = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textView),FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textView),FALSE);
	gtk_widget_set_size_request(textView,250,550);

	textViewBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	if(indexFlag == FAIL){
		sprintf(buf,"Index not created.\nFirst create Index.\nThen Search.\n");
	}else{
		readFile("INDEX/STATUS.txt",buf);
	}
	gtk_text_buffer_set_text(textViewBuffer,buf,-1);


	gtk_fixed_put(GTK_FIXED(fixed), textView, X_INDEX, Y_LOWER + 30);


	// Final commands
	g_signal_connect_swapped(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), G_OBJECT(window));
	gtk_widget_show_all(window);
	gtk_main();	
	gdk_threads_enter();
	return 0;
}
