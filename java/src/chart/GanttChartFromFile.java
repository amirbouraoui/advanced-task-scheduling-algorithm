package chart;

import javafx.scene.layout.AnchorPane;
import schedule.Machine;
import schedule.Task;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.time.Duration;
import java.time.Instant;
import java.util.HashMap;
import java.util.Map;

import com.flexganttfx.model.Layer;
import com.flexganttfx.model.layout.GanttLayout;
import com.flexganttfx.view.GanttChart;
import com.flexganttfx.view.graphics.GraphicsBase;
import com.flexganttfx.view.timeline.Timeline;

import javafx.scene.paint.Color;

public class GanttChartFromFile {
	
	private Map<Integer, Color> taskColors = new HashMap<>();

	public void createGanttChart(AnchorPane ganttChartContainer, String filePath) throws IOException {
	    Machine root = new Machine("Result");
	    GanttChart<Machine> gantt = new GanttChart<>(root);

	    Layer layer = new Layer("Tasks");
	    gantt.getLayers().add(layer);

	    Instant earliestStartTime = null;
	    Instant latestEndTime = null;

	    try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
	        String line;
	        Machine currentMachine = null;

	        // Skip the first line (number of machines)
	        reader.readLine();

	        while ((line = reader.readLine()) != null) {
	            if (line.startsWith("M")) {
	                currentMachine = new Machine(line);
	                gantt.getRoot().getChildren().add(currentMachine);
	            } else {
	                String[] tokens = line.split(" ");
	                if (tokens.length < 3) {
	                    continue;
	                }
	                int taskId = Integer.parseInt(tokens[2]);
	                int start = Integer.parseInt(tokens[0]);
	                int end = Integer.parseInt(tokens[1]);
	                
	                Instant taskStartTime = Instant.now().plus(Duration.ofHours(start));
	                Instant taskEndTime = Instant.now().plus(Duration.ofHours(end));

	                // Update earliest start time and latest end time
	                if (earliestStartTime == null || taskStartTime.isBefore(earliestStartTime)) {
	                    earliestStartTime = taskStartTime;
	                }
	                if (latestEndTime == null || taskEndTime.isAfter(latestEndTime)) {
	                    latestEndTime = taskEndTime;
	                }
	                
					Color taskColor = taskColors.get(taskId);
					if (taskColor == null) {
						taskColor = Color.color(Math.random(), Math.random(), Math.random()); // generate a new color
						taskColors.put(taskId, taskColor);
					}

	                Task task = new Task(taskId, taskStartTime, taskEndTime, taskColor);
	                currentMachine.addActivity(layer, task);
	            }
	        }
	    }

	    Timeline timeline = gantt.getTimeline();

	    if (earliestStartTime != null && latestEndTime != null) {
	        Duration duration = Duration.between(earliestStartTime, latestEndTime);
	        timeline.showRange(earliestStartTime, duration);

	        // Fix for the larger tasks
	        timeline.showTime(earliestStartTime.plus(duration.dividedBy(9)), true);
	    }


        GraphicsBase<Machine> graphics = gantt.getGraphics();
        CustomTaskRenderer taskRenderer = new CustomTaskRenderer(graphics, "Task Renderer");
        graphics.setActivityRenderer(Task.class, GanttLayout.class, taskRenderer);

	    AnchorPane.setTopAnchor(gantt, 0.0);
	    AnchorPane.setRightAnchor(gantt, 0.0);
	    AnchorPane.setBottomAnchor(gantt, 0.0);
	    AnchorPane.setLeftAnchor(gantt, 0.0);

	    root.setExpanded(true);
	    ganttChartContainer.getChildren().add(gantt);
	}
	
}
