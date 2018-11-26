import javafx.application.Application;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.Pane;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.Scene;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.scene.input.MouseEvent;
import BlockGrid;

public class BoskBlockEd extends Application
{
	public static void main( String args[] )
	{
		launch( args );
	}

	@Override
	public void start( Stage newStage ) throws Exception
	{
		stage = newStage;
		stage.setTitle( "Hello" );
		stage.setMaximized( true );
		rootNode = new FlowPane();
		Scene scene = new Scene( rootNode );
		blockPane = new Pane();
		rootNode.getChildren().addAll( generateMenuBar(), blockPane );
		stage.setScene( scene );
		stage.show();
	}

	private VBox generateMenuBar()
	{
		VBox vbox = new VBox();
		vbox.prefWidthProperty().bind( stage.widthProperty() );
		MenuBar menuBar = new MenuBar();
		menuBar.getMenus().add( generateFileMenu() );
		vbox.getChildren().add( menuBar );
		return vbox;
	}

	private Menu generateFileMenu()
	{
		Menu fileMenu = new Menu( "File" );
		fileMenu.getItems().add( generateNewMenuItem() );
		return fileMenu;
	}

	private MenuItem generateNewMenuItem()
	{
		MenuItem newMenuItem = new MenuItem( "New" );
		newMenuItem.setOnAction( e -> { setupNewTileset(); } );
		return newMenuItem;
	}

	private void setupNewTileset()
	{
		createBlockGrid();
	}

	private void createBlockGrid()
	{
		blockGrid = new BlockGrid();
		blockPane.getChildren().removeAll();
		blockPane.getChildren().add( blockGrid );
	}

	private Stage stage;
	private FlowPane rootNode;
	private Pane blockPane;
	private BlockGrid blockGrid;
}
