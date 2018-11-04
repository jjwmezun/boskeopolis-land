import javafx.application.Application;
import javafx.scene.control.Button;
import javafx.scene.control.PopupControl;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.layout.FlowPane;
import javafx.scene.Scene;
import javafx.stage.DirectoryChooser;
import javafx.stage.Stage;
import java.io.File;
import java.nio.file.Files;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.io.IOException;

public class BoskBlockEd extends Application
{
	public static void main( String args[] )
	{
		blocksList = new String[ 400 ];
		for ( int i = 0; i < 400; i++ )
		{
			blocksList[ i ] = null;
		}

		launch( args );
	}

	@Override
	public void start( Stage stage ) throws Exception
	{
		stage.setTitle( "Hello" );
		stage.setMaximized( true );
		FlowPane rootNode = new FlowPane();
		Scene scene = new Scene( rootNode );
		stage.setScene( scene );

		Button loadFolderButton = new Button( "Load folder..." );
		loadFolderButton.setOnAction
		(
			new EventHandler<ActionEvent> ()
			{
				public void handle( ActionEvent event )
				{
					DirectoryChooser directoryChooser = new DirectoryChooser();
					File directory = directoryChooser.showDialog( new PopupControl() );
					for ( int i = 0; i < 400; i++ )
					{
						String formattedNumber = String.format( "%03d", i );
						String fileName = formattedNumber + ".json";
						File block = new File( directory, fileName );
						if ( block.canRead() )
						{
							try
							{
								blocksList[ i ] = "";
								List<String> lines = Files.readAllLines( block.toPath(), StandardCharsets.UTF_8 );
								for ( String line : lines )
								{
									blocksList[ i ] += line;
								}
							}
							catch ( IOException except )
							{
							}
						}
					}
				}
			}
		);
		rootNode.getChildren().add( loadFolderButton );

		stage.show();
	}

	static private String[] blocksList;
}
