package schedule;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class TaskFileProcessor {

    private File inputFile;
    private int taskAmount;
    private double taskAVG;

    public TaskFileProcessor(File selectedFile) {
        this.inputFile = selectedFile;
        processData();
    }

    private void processData() {
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            String line;
            int totalDuration = 0;
            int taskCount = 0;

            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(" ");
                int duration = Integer.parseInt(parts[1]);
                totalDuration += duration;
                taskCount++;
            }

            this.taskAmount = taskCount;
            this.taskAVG = (double) totalDuration / taskCount;

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public int getTaskAmount() {
        return taskAmount;
    }

    public double getTaskAVG() {
        return taskAVG;
    }
}
